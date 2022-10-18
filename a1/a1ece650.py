#!/usr/bin/env python3
import sys
import re
import math
import subprocess

from subroutines import intersect, pp, vertices_sort

# ************************************************************
# defines
# ************************************************************

name_rx = '\"[a-zA-Z\s{0,1}]+\"'
num_rx = '-?\d+'
coord_rx = r'\('+num_rx+','+num_rx+'\)'

cmd_a_rx = 'add\s'+name_rx+'\s('+coord_rx+'\s){2,}$'
cmd_m_rx = 'mod\s'+name_rx+'\s('+coord_rx+'\s){2,}$'
cmd_r_rx = 'rm\s'+name_rx+'$'
cmd_gg_rx = 'gg$'

cmd_a_chk = re.compile(cmd_a_rx)
cmd_m_chk = re.compile(cmd_m_rx)
cmd_r_chk = re.compile(cmd_r_rx)
cmd_gg_chk = re.compile(cmd_gg_rx)


class Street():
    street_names = []
    street_coords = []

    def get_name(x):
        name = re.findall(name_rx, x)[0].upper().strip()
        if name.replace('"', '')[0] == ' ' or name.replace('"', '')[-1] == ' ' or '  ' in name:
            print("Error: Check street name")
            return None
        elif name.replace(' ', '').replace('"', '').isalpha():
            return name
        else:
            print("Error: Check street name")
            return None

    def get_coords(x):
        coords = [tuple([float(num) for num in re.findall(num_rx, coord)])
                  for coord in re.findall(coord_rx, x)]
        return coords

    def add_to_list(line):
        Street.street_names.append(Street.get_name(line))
        Street.street_coords.append(Street.get_coords(line))


class Graph():
    vertices = []
    edges = []

    def clear_graph():
        Graph.vertices = []
        Graph.edges = []

    def show_graph():
        print('V = {')
        if Graph.vertices:
            for index, item in enumerate(Graph.vertices):
                print('{}: {}'.format(index+1, item))
        print('}\nE = {')
        if Graph.edges:
            for index, item in enumerate(Graph.edges):
                if index + 1 < len(Graph.edges):
                    print('<{},{}>,'.format(item[0], item[1]))
                else:
                    print('<{},{}>'.format(item[0], item[1]))
        print('}')


# ************************************************************
# main
# ************************************************************

def main():
    while True:
        line = sys.stdin.readline()

        if line == "":
            break
        elif '-0' in line:
            print('Error: Check format of coordinates')
        elif not line.isspace() and line[-2] == " ":
            print('Error: Check trailing spaces')

        elif cmd_a_chk.match(line):
            name = Street.get_name(line)
            coord = Street.get_coords(line)
            if name != None and coord != None:
                if name not in Street.street_names:
                    Street.add_to_list(line)
                else:
                    print('Error: Street already exists, try mod command!')

        elif cmd_m_chk.match(line):
            name = Street.get_name(line)
            coord = Street.get_coords(line)
            if name != None and coord != None:
                if name in Street.street_names:
                    Street.street_coords[Street.street_names.index(
                        name)] = Street.get_coords(line)
                else:
                    print('Error: Street does not exist. Try add command!')

        elif cmd_r_chk.match(line):
            name = Street.get_name(line)
            if name in Street.street_names:
                del Street.street_coords[Street.street_names.index(name)]
                Street.street_names.remove(name)
            else:
                print('Error: Street does not exist!')

        elif cmd_gg_chk.match(line):
            intersection_list = []
            remove_extra_edges = []
            Graph.clear_graph()
            if len(Street.street_coords) > 1:
                for st_coord1 in Street.street_coords:
                    for st_coord2 in Street.street_coords:
                        if st_coord1 != st_coord2:
                            a = st_coord1
                            b = st_coord2
                            if len(a) > 1 and len(b) > 1:
                                for xi, xitem in enumerate(a):
                                    for yi, yitem in enumerate(b):
                                        intersect_ans = intersect(
                                            xitem, a[xi+1], yitem, b[yi+1])
                                        if intersect_ans != None:
                                            if intersect_ans not in Graph.vertices:
                                                intersection_list.append(
                                                    intersect_ans)
                                                Graph.vertices.append(
                                                    intersect_ans)
                                            temp_list = [
                                                xitem, a[xi+1], yitem, b[yi+1]]
                                            for x in temp_list:
                                                if x not in Graph.vertices:
                                                    Graph.vertices.append(x)
                                                    Graph.edges.append([Graph.vertices.index(
                                                        intersect_ans)+1, Graph.vertices.index(x)+1])
                                        if yi+2 == len(b):
                                            break
                                    if xi+2 == len(a):
                                        break
                # Check if there are multiple vertices on valid edges

                for index, item in enumerate(Graph.edges):
                    extra_intersections = []
                    for item2 in intersection_list:
                        x1 = Graph.vertices[item[0]-1][0]
                        x2 = Graph.vertices[item[1]-1][0]
                        y1 = Graph.vertices[item[0]-1][1]
                        y2 = Graph.vertices[item[1]-1][1]

                        x_evaluation = item2[0] > min(
                            x1, x2) and item2[0] < max(x1, x2)
                        y_evaluation = item2[1] >= min(
                            y1, y2) and item2[1] <= max(y1, y2)
                        x_evaluation2 = item2[0] >= min(
                            x1, x2) and item2[0] <= max(x1, x2)
                        y_evaluation2 = item2[1] > min(
                            y1, y2) and item2[1] < max(y1, y2)

                        if x1-x2 != 0 and (item2[0]-x1) != 0 and (x2-item2[0]):
                            slope1 = (item2[1]-y1)/(item2[0]-x1)
                            slope2 = (y2-item2[1])/(x2-item2[0])
                        else:
                            slope1 = slope2 = 0

                        if ((x_evaluation and y_evaluation) or (x_evaluation2 and y_evaluation2)):
                            if int(round(slope1)) == int(round(slope2)):
                                extra_intersections.append(item2)

                    if len(extra_intersections) == 1:
                        remove_extra_edges.append(item)
                        Graph.edges.append([Graph.vertices.index(
                            extra_intersections[0])+1, item[0]])
                        Graph.edges.append([Graph.vertices.index(
                            extra_intersections[0])+1, item[1]])
                    elif len(extra_intersections) > 1:
                        pair = [item[0], item[1]]
                        pair.sort()
                        remove_extra_edges.append(item)
                        ans = vertices_sort(extra_intersections)
                        source_first = [pair[0], Graph.vertices.index(
                            extra_intersections[ans[0]])+1]
                        last_dest = [pair[1], Graph.vertices.index(
                            extra_intersections[ans[-1]])+1]
                        if source_first not in Graph.edges and last_dest not in Graph.edges:
                            Graph.edges.append(source_first)
                            Graph.edges.append(last_dest)
                        for index, item in enumerate(ans):
                            src = Graph.vertices.index(
                                extra_intersections[item])
                            des = Graph.vertices.index(
                                extra_intersections[ans[index+1]])
                            if src != des and [src, des] not in Graph.edges and [des, src] not in Graph.edges:
                                Graph.edges.append([src+1, des+1])
                            if index+1 == len(ans)-1:
                                break
                    if index + 2 == len(Graph.edges):
                        break
                for x in remove_extra_edges:
                    Graph.edges.remove(x)
            Graph.show_graph()

        else:
            print('Error: Invalid command')

    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == "__main__":
    main()
