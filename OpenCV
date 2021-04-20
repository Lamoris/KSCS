import math
import cv2 as cv

def LaneFiltering(lines):
    
    if lines is None:
        return None
    
    Lanes = []
    
    for i in range(0,len(lines)):
        l = lines[i][0]
        x1,y1,x2,y2 = l[0],l[1],l[2],l[3]
        if x1 == x2:
            Lanes.append([x1,y1,x2,y2])
            continue
        angle = math.atan((y2-y1)/(x2-x1))*180/math.pi
        if abs(angle) > 10:
            Lanes.append([x1,y1,x2,y2])
    
    return Lanes

def SearchLines(img):
    if img is None:
        return None
    
    gray = cv.cvtColor(img,cv.COLOR_BGR2GRAY)
    gray = cv.GaussianBlur(gray,(7,7),0)
    edges = cv.Canny(gray,40, 100)
    
    cv.imshow('edges',edges)
    
    #Probabilistic Line Transform
    #https://docs.opencv.org/master/dd/d1a/group__imgproc__feature.html#ga8618180a5948286384e3b7ca02f6feeb
    lines = cv.HoughLinesP(edges, 1, math.pi / 180, threshold=80, lines=None, minLineLength=20, maxLineGap=20)
    
    edges = cv.cvtColor(edges,cv.COLOR_GRAY2BGR)
    if lines is not None:
        for i in range(0, len(lines)):
            l = lines[i][0]
            cv.line(edges, (l[0], l[1]), (l[2], l[3]), (255,0,255), 3, cv.LINE_AA)
    
    cv.imshow('HoughLinesP',edges)
    
    return lines
    

def LaneRecognition():
    
    img = cv.imread('road-220058_640.jpg')
    cv.imshow('img',img)
    
    lines = SearchLines(img)
    Lane = LaneFiltering(lines)
        
    if Lane is not None:
        for i in range(0, len(Lane)):
            cv.line(img, (Lane[i][0], Lane[i][1]), (Lane[i][2], Lane[i][3]), (0,255,0), 3, cv.LINE_AA)
    
    cv.imshow('Lane',img)

    cv.waitKey(0)
    cv.destroyAllWindows()
