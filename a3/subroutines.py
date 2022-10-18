import math
import copy

def pp(x):
    """Returns a pretty-print string representation of a number.
       A float number is represented by an integer, if it is whole,
       and up to two decimal places if it isn't
    """
    if isinstance(x, float):
        if x.is_integer():
            return str(int(x))
        else:
            return "{0:.2f}".format(x)
    return str(x)

class point(object):
    """A point in a two dimensional space"""
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    def __repr__(self):
        return '(' + pp(self.x) + ', ' + pp(self.y) + ')'


class line(object):
    """A line between two points"""
    def __init__(self, src, dst):
        self.src = src
        self.dst = dst

    def __repr__(self):
        return '['+ str(self.src) + '-->' + str(self.dst) + ']'


def intersect_coords (l1, l2):
    """Returns a point at which two lines intersect"""
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y

    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
    den = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    if den != 0:
        xcoor =  xnum / den
        ycoor = ynum / den
        x_evaluation = xcoor>=min(x1,x2) and xcoor<=max(x1,x2) and xcoor>=min(x3,x4) and xcoor<=max(x3,x4)
        y_evaluation = ycoor>=min(y1,y2) and ycoor<=max(y1,y2) and ycoor>=min(y3,y4) and ycoor<=max(y3,y4)
        if x_evaluation and y_evaluation:
            return (round(xcoor,2), round(ycoor,2))
        else:
            return None
    else:
        return None

def intersect(p1, p2, p3, p4):
    eval(str(p1))
    eval(str(p2))
    eval(str(p3))
    eval(str(p4))
    if (p1 == p3 and p2 == p4) or (p1 == p4 and p2 == p3):
        return (p1,p2)
    else:
        l1 = line(point(p1[0],p1[1]), point(p2[0], p2[1]))
        l2 = line(point(p3[0], p3[1]), point(p4[0], p4[1]))
        return intersect_coords(l1, l2)

def vertices_sort(x_list):
    abs_list = []
    abs_index_list = []
    for x in x_list:
        abs_list.append(math.sqrt(x[0]**2+x[1]**2))
    temp = copy.deepcopy(abs_list)
    abs_list.sort()
    for x in abs_list:
        for index, item in enumerate(temp):
            if x == item:
                abs_index_list.append(index)
    return abs_index_list

 