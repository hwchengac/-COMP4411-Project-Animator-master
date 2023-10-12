#include "CatmullRomCurveEvaluator.h"
#include <assert.h>
#include <iostream>

using namespace std;

void CatmullRomCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();

	std::vector<Point> tempPtvCtrlPts = vector<Point>(ptvCtrlPts.begin(), ptvCtrlPts.end());
	
	// Adding phantom points

	if (bWrap) {
		//cout << "is BWrap" << endl;
		// Phantom point for Wrapping + Phantom point
		int ptsSize = ptvCtrlPts.size();
		tempPtvCtrlPts.insert(tempPtvCtrlPts.begin(), Point(ptvCtrlPts.at(ptsSize - 1).x - fAniLength, ptvCtrlPts.at(ptsSize - 1).y));
		tempPtvCtrlPts.insert(tempPtvCtrlPts.begin(), Point(ptvCtrlPts.at(ptsSize - 2).x - fAniLength, ptvCtrlPts.at(ptsSize - 2).y));
		// Phantom point for Wrapping + Phantom point
		tempPtvCtrlPts.push_back(Point(ptvCtrlPts.at(0).x + fAniLength, ptvCtrlPts.at(0).y));
		tempPtvCtrlPts.push_back(Point(ptvCtrlPts.at(1).x + fAniLength, ptvCtrlPts.at(1).y));
	} else {
		//cout << "not BWrap" << endl;
		// Phantom point
		tempPtvCtrlPts.insert(tempPtvCtrlPts.begin(), Point(0, ptvCtrlPts.front().y));
		tempPtvCtrlPts.push_back(Point(fAniLength, ptvCtrlPts.back().y));
		ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts.front().y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts.back().y));
	}

	// Loop for each section
	for (int i = 0; i < tempPtvCtrlPts.size() - 3; i++) {
		Point point1 = tempPtvCtrlPts[i];
		Point point2 = tempPtvCtrlPts[i + 1];
		Point point3 = tempPtvCtrlPts[i + 2];
		Point point4 = tempPtvCtrlPts[i + 3];
		//cout << "1. x: " << point1.x << " y: " << point1.y << endl;
		//cout << "2. x: " << point2.x << " y: " << point2.y << endl;
		//cout << "3. x: " << point3.x << " y: " << point3.y << endl;
		//cout << "4. x: " << point4.x << " y: " << point4.y << endl;
		//Vec4f VectorFour1 = BEZIER_MATRIX * Vec4f(point1.x, point2.x, point3.x, point4.x);
		std::vector<Point> sectionPoints;
		Vec4f VectorFour1 = Vec4f(CAMULLROM_MATRIX[0][0] * point1.x + CAMULLROM_MATRIX[0][1] * point2.x + CAMULLROM_MATRIX[0][2] * point3.x + CAMULLROM_MATRIX[0][3] * (point4.x),
			CAMULLROM_MATRIX[1][0] * point1.x + CAMULLROM_MATRIX[1][1] * point2.x + CAMULLROM_MATRIX[1][2] * point3.x + CAMULLROM_MATRIX[1][3] * (point4.x),
			CAMULLROM_MATRIX[2][0] * point1.x + CAMULLROM_MATRIX[2][1] * point2.x + CAMULLROM_MATRIX[2][2] * point3.x + CAMULLROM_MATRIX[2][3] * (point4.x),
			CAMULLROM_MATRIX[3][0] * point1.x + CAMULLROM_MATRIX[3][1] * point2.x + CAMULLROM_MATRIX[3][2] * point3.x + CAMULLROM_MATRIX[3][3] * (point4.x));
		Vec4f VectorFour2 = Vec4f(CAMULLROM_MATRIX[0][0] * point1.y + CAMULLROM_MATRIX[0][1] * point2.y + CAMULLROM_MATRIX[0][2] * point3.y + CAMULLROM_MATRIX[0][3] * point4.y,
			CAMULLROM_MATRIX[1][0] * point1.y + CAMULLROM_MATRIX[1][1] * point2.y + CAMULLROM_MATRIX[1][2] * point3.y + CAMULLROM_MATRIX[1][3] * point4.y,
			CAMULLROM_MATRIX[2][0] * point1.y + CAMULLROM_MATRIX[2][1] * point2.y + CAMULLROM_MATRIX[2][2] * point3.y + CAMULLROM_MATRIX[2][3] * point4.y,
			CAMULLROM_MATRIX[3][0] * point1.y + CAMULLROM_MATRIX[3][1] * point2.y + CAMULLROM_MATRIX[3][2] * point3.y + CAMULLROM_MATRIX[3][3] * point4.y);
		for (float j = point1.x; j < point4.x; j += 0.5) {
			float t = (j - point1.x) / (point4.x - point1.x);
			Vec4f VectorT = Vec4f(t * t * t, t * t, t, 1);
			float newX = VectorT * VectorFour1;
			float newY = VectorT * VectorFour2;
			//cout << "newX: " << newX << endl;
			// Check if newX is larger than previous point.x and smaller than the upper bound
			if ((sectionPoints.empty() || newX > sectionPoints.back().x) && newX < point3.x) {
				sectionPoints.push_back(Point(newX, newY));
			}
		}
		ptvEvaluatedCurvePts.push_back(tempPtvCtrlPts[i + 1]);
		ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.end(), sectionPoints.begin(), sectionPoints.end());
		ptvEvaluatedCurvePts.push_back(tempPtvCtrlPts[i + 2]);
	}
}
