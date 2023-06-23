import sys; import importlib.util; import os; import subprocess; import argparse

for package in ['numpy', 'scipy', 'pyvista', 'shapely']:
	if importlib.util.find_spec(package) is None:
		subprocess.check_call([sys.executable, "-m", "pip", "install", package])

import pyvista as pv
import numpy as np
import shapely as sp

from math import sqrt, cos, sin, pi, atan2, floor, hypot
from scipy.spatial import ConvexHull
from scipy.interpolate import interp1d
from shapely import distance
from shapely.geometry import Polygon, Point, LinearRing, LineString, MultiPoint
from shapely.ops import transform, nearest_points, split
from shapely.affinity import rotate

def matan(point):
    return atan2(point.y, point.x) if atan2(point.y, point.x) > 0 else atan2(point.y, point.x)+2*pi

def GetContourFromOBJ(filename):
    return LinearRing(MultiPoint(np.delete(pv.read(filename).project_points_to_plane().points, 2, 1)).convex_hull.boundary.coords)

def ClockwiseAngleAndDistance(point):
    origin, refvec = [0,0], [1,0]
    vector = [point[0]-origin[0], point[1]-origin[1]]
    lenvector = hypot(vector[0], vector[1])
    if lenvector == 0: return -pi, 0
    normalized = [vector[0]/lenvector, vector[1]/lenvector]
    dotprod  = normalized[0]*refvec[0] + normalized[1]*refvec[1]
    diffprod = refvec[1]*normalized[0] - refvec[0]*normalized[1]
    angle = atan2(diffprod, dotprod)
    if angle < 0: return 2*pi+angle, lenvector
    return angle, lenvector

def GetCornerIndices(pts):
    a = np.array([[pts[i].x, pts[i].y] for i in range(len(pts))])
    dx_dt, dy_dt = np.gradient(a[:, 0]), np.gradient(a[:, 1])
    velocity = np.array([ [dx_dt[i], dy_dt[i]] for i in range(dx_dt.size)])
    ds_dt = np.sqrt(dx_dt * dx_dt + dy_dt * dy_dt)
    tangent = np.array([1/ds_dt]).transpose() * velocity
    deriv_tangent_x, deriv_tangent_y = np.gradient(tangent[:, 0]), np.gradient(tangent[:, 1])
    dT_dt = np.array([ [deriv_tangent_x[i], deriv_tangent_y[i]] for i in range(deriv_tangent_x.size)])
    length_dT_dt = np.sqrt(deriv_tangent_x * deriv_tangent_x + deriv_tangent_y * deriv_tangent_y)
    normal = np.array([1/length_dT_dt]).transpose() * dT_dt
    d2s_dt2, d2x_dt2, d2y_dt2 = np.gradient(ds_dt), np.gradient(dx_dt), np.gradient(dy_dt)
    curvature = np.abs(d2x_dt2 * dy_dt - dx_dt * d2y_dt2) / (dx_dt * dx_dt + dy_dt * dy_dt)**1.5
    return [idx for idx, val in enumerate(curvature) if val>=np.max(curvature)/2]

def ShiftConcatenate(data, shift):
    data = data.copy()
    for _ in range(shift): data.append(data.pop(0))
    return data

def GetSmoothedPear(pear):
    pear_original_pts, pts = [Point(pear.coords[i][0], pear.coords[i][1]) for i in range(len(pear.coords))], []
    for pt in pear_original_pts: pts.append([pt.x, pt.y])
    pts = sorted(pts, key=ClockwiseAngleAndDistance)
    pear_original__pts = [Point(pt[0], pt[1]) for pt in pts]
    corner_indices = GetCornerIndices(pear_original_pts)
    if len(corner_indices) > 0: 
        pear_original_shifted_pts = ShiftConcatenate(pear_original_pts, corner_indices[0])
    pear_original_shifted_pts, pts = list(set(pear_original_shifted_pts)), []
    for pt in pear_original_shifted_pts: pts.append([pt.x, pt.y])
    pts = sorted(pts, key=ClockwiseAngleAndDistance)
    pear_original_shifted_pts = [Point(pt[0], pt[1]) for pt in pts]
    corner_indices = GetCornerIndices(pear_original_shifted_pts)
    if len(corner_indices) > 0: 
        pear_original_shifted_pts = ShiftConcatenate(pear_original_shifted_pts, corner_indices[0])
    x, y = [pt.x for pt in pear_original_shifted_pts[1:]], [pt.y for pt in pear_original_shifted_pts[1:]]
    points = np.array([x, y]).T
    distance = np.cumsum( np.sqrt(np.sum( np.diff(points, axis=0)**2, axis=1 )) )
    distance = np.insert(distance, 0, 0)/distance[-1]
    interpolator =  interp1d(distance, points, kind='cubic', axis=0)
    interpolated_points = interpolator(np.linspace(0, 1, 1000))
    pts = [Point(pt[0], pt[1]) for pt in interpolated_points]
    pts.append(pear_original_shifted_pts[0])
    return LinearRing(pts)

def Contour2Points(contour, begin_angle, n):
    contour_pts = []
    for i in range(n):
        angle = begin_angle + (i/n)*2*pi
        intersection = contour.intersection(LineString([(0,0), (30*cos(angle), 30*sin(angle))]))
        if intersection.geom_type == 'Point': contour_pts.append(intersection)
        else: contour_pts.append(intersection.geoms[-1])
    return contour_pts

def GetBaloonedContour(contour, baloon_distance):
    return LinearRing(LineString(contour).offset_curve(baloon_distance, quad_segs=int(2**5), join_style=2, mitre_limit=5.0))

def GetOrderedIntersectionPoints(geom1, geom2):
    intersection, pts, spts = geom1.intersection(geom2), [], []
    if intersection.geom_type == 'LineString':
        return [], []
    else:
        for geom in intersection.geoms: pts.append([geom.x, geom.y])
        pts = sorted(pts, key=ClockwiseAngleAndDistance)
        for pt in reversed(pts): spts.append(Point(pt[0], pt[1]))
        angle1, angle2 = matan(spts[0]), matan(spts[1])
        line = LineString([(0,0), (30*cos((angle1+angle2)/2), 30*sin((angle1+angle2)/2))])
        pt1, pt2 = geom1.intersection(line), geom2.intersection(line)
        if Point(0,0).distance(pt1) < Point(0,0).distance(pt2): spts.append(spts.pop(0))
        return spts, [matan(pt) for pt in spts]

def GetSimpleOrderedIntersectionPoints(geom1, geom2):
    intersection, pts, spts = geom1.intersection(geom2), [], []
    if intersection.geom_type == 'LineString':
        return [], []
    elif intersection.geom_type == 'Point':
        return [intersection], [matan(intersection)]
    else:
        for geom in intersection.geoms: pts.append([geom.x, geom.y])
        pts = sorted(pts, key=ClockwiseAngleAndDistance)
        for pt in reversed(pts): spts.append(Point(pt[0], pt[1]))
        return spts, [matan(pt) for pt in spts]

def GetBaloonedContourWithContainPercentage(geom1, geom2, n, contain_percent):
    geom1_pts, geom2_pts = Contour2Points(geom1, 0, n),  Contour2Points(geom2, 0, n)
    dsts = [geom1_pts[i].distance(geom2_pts[i]) for i in range(n)]
    hist, bin_edges = np.histogram(dsts, bins=floor(n/4))
    sum, percent, whole_sum, i_edge = 0, 0, hist.sum(), 0
    for i in range(len(hist)):
        sum += hist[i]
        percent = sum / whole_sum
        if percent > contain_percent:
            i_edge = i-1
            break
    blgeom2 = GetBaloonedContour(geom2, np.min(dsts)+(i_edge/len(hist))*(np.max(dsts)-np.min(dsts)))
    spts, intersection_angles = GetOrderedIntersectionPoints(geom1, blgeom2)
    return blgeom2, spts

def GetReperIndices(angles, intersection_angles):
    indices = []
    for angle in intersection_angles:
        for i in range(len(angles)):
            if angles[i] > angle:
                indices.append(i)
                break
    return indices, len(indices)

def GetMaxDistance(geom1_pts, geom2):
    dst_max = 0.0
    for i in range(len(geom1_pts)):
        pt1, pt2 = nearest_points(geom2, geom1_pts[i])
        dst1, dst2 = geom1_pts[i].distance(pt1), geom1_pts[i].distance(pt2)
        dst = max(dst1, dst2)
        if dst > dst_max: dst_max = dst
    return dst_max

def GetNextCycleContour(geom1, geom2, dst_delta):
    blgeom2, dst = LineString(geom2).offset_curve(-dst_delta, join_style=2), dst_delta
    spts, val = GetSimpleOrderedIntersectionPoints(geom1, blgeom2)
    return blgeom2, spts, dst

def GetNextBrutingCycle(trace1_pts, trace2_pts, dst):
    assert len(trace1_pts) == len(trace2_pts)
    trace_pts = []
    for i in range(len(trace1_pts)):
        x1, y1 = trace1_pts[i].x, trace1_pts[i].y
        x2, y2 = trace2_pts[i].x, trace2_pts[i].y
        nrm = sqrt((x2-x1)**2+(y2-y1)**2)
        assert abs(nrm) > 1e-14
        trace_pts.append(Point(x1+dst*(x2-x1)/nrm, y1+dst*(y2-y1)/nrm))
    return trace_pts

def main():
	parser = argparse.ArgumentParser(description='Swing bruting algorithm')
	parser.add_argument('input', metavar='input.info', type=str, nargs=1, help='input filename')
	with open(parser.parse_args().input[0], 'r') as f:
		input_data = f.readlines()[0].split(' ')

	R = float(input_data[0]); n = int(input_data[1]); r = float(input_data[2]); contain_percent = float(input_data[3]);
	max_delta_distance = float(input_data[4]); cycle_count = int(input_data[5])
	rough_filename = input_data[6]; diamond_filename = input_data[7]; output = input_data[8]
	disk_delta_translation = max_delta_distance / cycle_count

	rough, pear = GetContourFromOBJ(rough_filename), GetSmoothedPear(GetContourFromOBJ(diamond_filename))
	angles = [2*pi*i/n for i in range(n)]
	blpear, val = GetBaloonedContourWithContainPercentage(rough, pear, n, contain_percent)
	spts, intersection_angles = GetOrderedIntersectionPoints(rough, blpear)
	indices, N = GetReperIndices(angles, intersection_angles)
	rough_pts, blpear_pts = Contour2Points(rough, 0, n), Contour2Points(blpear, 0, n)

	cycle_traces, sptss = [], []
	for j in range(0, N, 2):
	    local_cycle_traces, local_sptss, i_min, i_max = [], [], indices[j], indices[j+1]
	    cycle_rough, cycle_ground = LineString(rough_pts[i_min:i_max]), LineString(blpear_pts[(i_min-50):(i_max+50)])
	    local_cycle_traces.append(cycle_ground); local_sptss.append([blpear_pts[i_min], blpear_pts[i_max-1]])
	    dst_max = GetMaxDistance(rough_pts[i_min:i_max], LineString(blpear_pts[i_min:i_max]))
	    local_cycle_count = floor(dst_max / max_delta_distance)+1
	    cycle_trace, spts, dst = GetNextCycleContour(cycle_rough, cycle_ground, max_delta_distance)
	    local_cycle_traces.append(cycle_trace); local_cycle_traces.append(cycle_trace); local_sptss.append(spts); local_sptss.append(spts)
	    for k in range(local_cycle_count):
	        cycle_trace, spts, dst = GetNextCycleContour(cycle_rough, local_cycle_traces[-1], max_delta_distance)
	        local_cycle_traces.append(cycle_trace); local_cycle_traces.append(cycle_trace); local_sptss.append(spts); local_sptss.append(spts)
	        if len(spts) == 0: break
	    local_cycle_traces.pop(); local_sptss.pop()
	    cycle_traces.append(local_cycle_traces); sptss.append(local_sptss)

	cut_cycle_traces = []
	for j in range(len(cycle_traces)):
	    local_cut_cycle_traces = []
	    for k in range(0, len(cycle_traces[j]), 2):
	        current_trace1, current_trace2, cut_pts = cycle_traces[j][k], cycle_traces[j][k+1], sptss[j][k]
	        current_trace1_pts = np.array([Point(pt[0], pt[1]) for pt in current_trace1.coords])
	        current_trace2_pts = np.array([Point(pt[0], pt[1]) for pt in current_trace2.coords])
	        cut_angle1, cut_angle2 = matan(cut_pts[0]), matan(cut_pts[-1])
	        angles1, angles2 = np.array([matan(pt) for pt in current_trace1_pts]), np.array([matan(pt) for pt in current_trace2_pts])
	        idx1, idx2 = np.where((cut_angle1 <= angles1)*(angles1 <= cut_angle2))[0], np.where((angles2 >= cut_angle1)*(angles2 <= cut_angle2))[0]
	        trace1, trace2, maxlen = LineString(current_trace1_pts[idx1].tolist()),  LineString(current_trace2_pts[idx2].tolist()), max(idx1.size,idx2.size)
	        local_angles = [(1-alpha)*cut_angle1+alpha*cut_angle2 for alpha in [i/maxlen for i in range(maxlen)]]
	        trace1_pts, trace2_pts = [], []
	        for angle in local_angles:
	            line = LineString([(0,0), (30*cos(angle), 30*sin(angle))])
	            pt1, pt2 = trace1.intersection(line), trace2.intersection(line)
	            if pt1.geom_type != 'Point' or pt2.geom_type != 'Point': continue
	            trace1_pts.append(pt1); trace2_pts.append(pt2)
	        local_cut_cycle_traces.append(trace1_pts); local_cut_cycle_traces.append(trace2_pts)
	    cut_cycle_traces.append(local_cut_cycle_traces)

	bruting_cycle_traces = []
	for j in range(len(cut_cycle_traces)):
	    bruting_cycle_trace = []
	    for k in range(0, len(cut_cycle_traces[j]), 2):
	        bruting_cycle_trace.append(cut_cycle_traces[j][k])
	        for i in range(cycle_count-1): 
	            bruting_cycle_trace.append(GetNextBrutingCycle(cut_cycle_traces[j][k], cut_cycle_traces[j][k+1], (i+1)*disk_delta_translation))
	        bruting_cycle_trace.append(cut_cycle_traces[j][k+1])
	    bruting_cycle_traces.append(bruting_cycle_trace)

	bruting_traces, disk_traces = [], []
	for j in range(len(bruting_cycle_traces)):
	    local_bruting_traces, local_disk_traces = [], []
	    for k in range(0, len(bruting_cycle_traces[j]), cycle_count+1):
	        for q in range(k, k+cycle_count):
	            trace1, trace2, local_local_bruting_trace, local_local_bruting_trace, local_len = bruting_cycle_traces[j][q], bruting_cycle_traces[j][q+1], [], [], len(bruting_cycle_traces[j][q])
	            local_bruting_traces.append(trace1); local_disk_traces.append([Point(pt[0], pt[1]) for pt in LineString(trace1).offset_curve(-R, join_style=2).coords])
	            for l in range(local_len): alpha = l/local_len; local_local_bruting_trace.append(Point((1-alpha)*trace1[l].x+alpha*trace2[l].x, (1-alpha)*trace1[l].y+alpha*trace2[l].y))
	            local_local_bruting_trace.reverse(); local_bruting_traces.append(local_local_bruting_trace); 
	            local_disk_traces.append([Point(pt[0], pt[1]) for pt in LineString(local_local_bruting_trace).offset_curve(R, join_style=2).coords])
	    local_bruting_traces.reverse(); local_disk_traces.reverse()
	    bruting_traces.append(local_bruting_traces); disk_traces.append(local_disk_traces)

	xvals, rangles, dvals, time, delta_time = [], [], [], [], []
	for j in range(len(disk_traces)):
	        local_xvals, local_rangles, local_dvals, local_time, local_delta_time = [], [], [], [], []
	        for q in (range(0, len(disk_traces[j]), 2*cycle_count)):
	                local_local_xvals, local_local_rangles, local_local_dvals, local_local_time = [], [], [], []
	                whole_length, cur_length, xstep, bruting_time = 0, 0, 0, 20.0
	                for k in range(q, q+2*cycle_count): whole_length += len(disk_traces[j][k])
	                delta_ttime = bruting_time/whole_length
	                for k in range(q, q+2*cycle_count):
	                        for l in range(len(disk_traces[j][k])):
	                                cur_length += 1
	                                angle = atan2(disk_traces[j][k][l].y, disk_traces[j][k][l].x)
	                                xstep += (1/len(disk_traces[j][k])) * disk_delta_translation/2
	                                xval = r-rotate(disk_traces[j][k][l], -angle, origin=Point(0.0,0.0), use_radians=True).x-xstep
	                                local_local_xvals.append(xval); local_local_rangles.append(angle*180/pi); 
	                                local_local_dvals.append(xstep); local_local_time.append(bruting_time*cur_length/whole_length)
	                local_xvals.append(local_local_xvals); local_rangles.append(local_local_rangles); 
	                local_dvals.append(local_local_dvals); local_time.append(local_local_time); local_delta_time.append(delta_ttime)
	        xvals.append(local_xvals); rangles.append(local_rangles); 
	        dvals.append(local_dvals); time.append(local_time); delta_time.append(local_delta_time)
	
	if os.path.exists(output): os.remove(output)
	for j in range(len(rangles)):
	        for q in range(len(rangles[j])):
	                with open(output, 'a') as f:
	                        f.write('; sector\ncycle\n{\n')
	                        f.write('\tRx_0 ' + str(round(rangles[j][q][0], 6)) + '\n')
	                        f.write('\tYs_0 ' + str(round(xvals[j][q][0], 6)) + '\n')
	                        f.write('\tYf_0 ' + str(round(dvals[j][q][0], 6)) + '\n')
	                        f.write('\tfeed\n\t{\n\t\t' + str(cycle_count) + ' ' + str(round(dvals[j][q][-1], 6)) + '\n\t}\n')
	                        f.write('\tT_step ' + str(round(delta_time[j][q], 6)) + '\n\tshape\n\t{\n')
	                        for k in range(1, len(xvals[j][q])):
	                                f.write('\t\t' + str(round(rangles[j][q][k], 6)) + ' ' + str(round(xvals[j][q][k], 6)) + '\n')
	                        f.write('\t}\n}\n\n')

	return 0

if __name__ == '__main__':
	main()