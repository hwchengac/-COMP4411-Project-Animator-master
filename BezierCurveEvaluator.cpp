#include "BezierCurveEvaluator.h"
#include <assert.h>
#include <iostream>

using namespace std;

int BezierCurveEvaluator::numberOfBezierCurve(int numOfPoints) const {
	// Return number of Bezier Curve
	if (numOfPoints > 3) {
		return (int) (numOfPoints - 1) / 3;
	}
	// Does not meet Bezier Curve Requirement
	return -1;
}

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{

	ptvEvaluatedCurvePts.clear();

	int iCtrlPtCount = ptvCtrlPts.size(), remainingCount = ptvCtrlPts.size();
	cout << "iCtrlPtCount: " << iCtrlPtCount << endl;
	if (numberOfBezierCurve(iCtrlPtCount) != -1 || (bWrap && numberOfBezierCurve(iCtrlPtCount + 1) != -1)) {
		cout << "Is Bezier" << endl;
		/*
		for (int i = 0; i < iCtrlPtCount; i++) {
			cout << "x: " << ptvCtrlPts[i].x << " y: " << ptvCtrlPts[i].y << endl;
		}
		*/
		float x = 0.0;
		float y1;

		if (bWrap) {
			// if wrapping is on, 
			if ((iCtrlPtCount) % 3 == 0) {
				// Do a wrapping Bezier Curve
				cout << "Wrapping BC" << endl;
				Point point1 = ptvCtrlPts[iCtrlPtCount - 3];
				Point point2 = ptvCtrlPts[iCtrlPtCount - 2];
				Point point3 = ptvCtrlPts[iCtrlPtCount - 1];
				Point point4 = ptvCtrlPts[0];
				cout << "1. x: " << point1.x << " y: " << point1.y << endl;
				cout << "2. x: " << point2.x << " y: " << point2.y << endl;
				cout << "3. x: " << point3.x << " y: " << point3.y << endl;
				cout << "4. x: " << point4.x << " y: " << point4.y << endl;
				//Vec4f VectorFour1 = BEZIER_MATRIX * Vec4f(point1.x, point2.x, point3.x, point4.x);
				Vec4f VectorFour1 = Vec4f(BEZIER_MATRIX[0][0] * point1.x + BEZIER_MATRIX[0][1] * point2.x + BEZIER_MATRIX[0][2] * point3.x + BEZIER_MATRIX[0][3] * (point4.x + fAniLength),
					BEZIER_MATRIX[1][0] * point1.x + BEZIER_MATRIX[1][1] * point2.x + BEZIER_MATRIX[1][2] * point3.x + BEZIER_MATRIX[1][3] * (point4.x + fAniLength),
					BEZIER_MATRIX[2][0] * point1.x + BEZIER_MATRIX[2][1] * point2.x + BEZIER_MATRIX[2][2] * point3.x + BEZIER_MATRIX[2][3] * (point4.x + fAniLength),
					BEZIER_MATRIX[3][0] * point1.x + BEZIER_MATRIX[3][1] * point2.x + BEZIER_MATRIX[3][2] * point3.x + BEZIER_MATRIX[3][3] * (point4.x + fAniLength));
				Vec4f VectorFour2 = Vec4f(BEZIER_MATRIX[0][0] * point1.y + BEZIER_MATRIX[0][1] * point2.y + BEZIER_MATRIX[0][2] * point3.y + BEZIER_MATRIX[0][3] * point4.y,
					BEZIER_MATRIX[1][0] * point1.y + BEZIER_MATRIX[1][1] * point2.y + BEZIER_MATRIX[1][2] * point3.y + BEZIER_MATRIX[1][3] * point4.y,
					BEZIER_MATRIX[2][0] * point1.y + BEZIER_MATRIX[2][1] * point2.y + BEZIER_MATRIX[2][2] * point3.y + BEZIER_MATRIX[2][3] * point4.y,
					BEZIER_MATRIX[3][0] * point1.y + BEZIER_MATRIX[3][1] * point2.y + BEZIER_MATRIX[3][2] * point3.y + BEZIER_MATRIX[3][3] * point4.y);
				cout << fAniLength << endl;
				cout << "BE " << VectorFour1[0] << " : " << VectorFour1[1] << " : " << VectorFour1[2] << " : " << VectorFour1[3] << endl;
				const float curveLength = fAniLength - (point1.x - point4.x);
				cout << "curveLength: " << curveLength << endl;
				for (float j = point1.x; j < point4.x + fAniLength; j += 0.05) {
					float t = (j - point1.x) / curveLength;
					Vec4f VectorT = Vec4f(t * t * t, t * t, t, 1);
					float newX = VectorT * VectorFour1;
					if (newX > fAniLength) {
						newX -= fAniLength;
					}
					float newY = VectorT * VectorFour2;
					ptvEvaluatedCurvePts.push_back(Point(newX, newY));
				}
				ptvEvaluatedCurvePts.push_back(Point(VectorFour1 * Vec4f(1, 1, 1, 1), VectorFour2 * Vec4f(1, 1, 1, 1)));
			}
			else {
				// Wrapping Linear
				cout << "Wrapping Linera curve" << endl;
				if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[iCtrlPtCount - 1].x > 0.0f) {
					y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) +
						ptvCtrlPts[iCtrlPtCount - 1].y * ptvCtrlPts[0].x) /
						(ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
				}
				else {
					y1 = ptvCtrlPts[0].y;
				}
				ptvEvaluatedCurvePts.push_back(Point(x, y1));
				float y2;
				x = fAniLength;
				if (bWrap)
					y2 = y1;
				else
					y2 = ptvCtrlPts[iCtrlPtCount - 1].y;

				ptvEvaluatedCurvePts.push_back(Point(x, y2));
			}
		}
		else {
			// if wrapping is off, make the first and last segments of
			// the curve horizontal.
			// Not Wrapping Linear
			y1 = ptvCtrlPts[0].y;
			ptvEvaluatedCurvePts.push_back(Point(x, y1));
			float y2;
			x = fAniLength;
			if (bWrap)
				y2 = y1;
			else
				y2 = ptvCtrlPts[iCtrlPtCount - 1].y;

			ptvEvaluatedCurvePts.push_back(Point(x, y2));
		}

		for (int curveNumber = 0; curveNumber < numberOfBezierCurve(iCtrlPtCount); curveNumber++) {
			Point point1 = ptvCtrlPts[0 + 3 * curveNumber];
			Point point2 = ptvCtrlPts[1 + 3 * curveNumber];
			Point point3 = ptvCtrlPts[2 + 3 * curveNumber];
			Point point4 = ptvCtrlPts[3 + 3 * curveNumber];
			//Vec4f VectorFour1 = BEZIER_MATRIX * Vec4f(point1.x, point2.x, point3.x, point4.x);
			Vec4f VectorFour1 = Vec4f(BEZIER_MATRIX[0][0] * point1.x + BEZIER_MATRIX[0][1] * point2.x + BEZIER_MATRIX[0][2] * point3.x + BEZIER_MATRIX[0][3] * point4.x,
				BEZIER_MATRIX[1][0] * point1.x + BEZIER_MATRIX[1][1] * point2.x + BEZIER_MATRIX[1][2] * point3.x + BEZIER_MATRIX[1][3] * point4.x,
				BEZIER_MATRIX[2][0] * point1.x + BEZIER_MATRIX[2][1] * point2.x + BEZIER_MATRIX[2][2] * point3.x + BEZIER_MATRIX[2][3] * point4.x,
				BEZIER_MATRIX[3][0] * point1.x + BEZIER_MATRIX[3][1] * point2.x + BEZIER_MATRIX[3][2] * point3.x + BEZIER_MATRIX[3][3] * point4.x);
			Vec4f VectorFour2 = Vec4f(BEZIER_MATRIX[0][0] * point1.y + BEZIER_MATRIX[0][1] * point2.y + BEZIER_MATRIX[0][2] * point3.y + BEZIER_MATRIX[0][3] * point4.y,
				BEZIER_MATRIX[1][0] * point1.y + BEZIER_MATRIX[1][1] * point2.y + BEZIER_MATRIX[1][2] * point3.y + BEZIER_MATRIX[1][3] * point4.y,
				BEZIER_MATRIX[2][0] * point1.y + BEZIER_MATRIX[2][1] * point2.y + BEZIER_MATRIX[2][2] * point3.y + BEZIER_MATRIX[2][3] * point4.y,
				BEZIER_MATRIX[3][0] * point1.y + BEZIER_MATRIX[3][1] * point2.y + BEZIER_MATRIX[3][2] * point3.y + BEZIER_MATRIX[3][3] * point4.y);
			const float curveLength = point4.x - point1.x;
			for (float j = point1.x; j < point4.x; j += 0.05) {
				float t = (j - point1.x) / curveLength;
				Vec4f VectorT = Vec4f(t * t * t, t * t, t, 1);
				float newX = VectorT * VectorFour1;
				float newY = VectorT * VectorFour2;
				ptvEvaluatedCurvePts.push_back(Point(newX, newY));
			}
			ptvEvaluatedCurvePts.push_back(Point(VectorFour1 * Vec4f(1, 1, 1, 1), VectorFour2 * Vec4f(1, 1, 1, 1)));
			remainingCount -= 3;
		}
		if (bWrap) {
			if (!((iCtrlPtCount) % 3 == 0)) {
				for (int j = 3 * numberOfBezierCurve(iCtrlPtCount); j < iCtrlPtCount; j++) {
					ptvEvaluatedCurvePts.push_back(ptvCtrlPts[j]);
				}
			}
		}
		else {
			if (!((iCtrlPtCount) % 3 == 1)) {
				for (int j = 3 * numberOfBezierCurve(iCtrlPtCount); j < iCtrlPtCount; j++) {
					ptvEvaluatedCurvePts.push_back(ptvCtrlPts[j]);
				}
			}
		}
		
	}
	else {
		cout << "No Bezier" << endl;
		ptvEvaluatedCurvePts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
		float x = 0.0;
		float y1;

		if (bWrap) {
			if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[iCtrlPtCount - 1].x > 0.0f) {
				y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) +
					ptvCtrlPts[iCtrlPtCount - 1].y * ptvCtrlPts[0].x) /
					(ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
			}
			else {
				y1 = ptvCtrlPts[0].y;
			}
		}
		else {
			// Not Wrapping Linear
			y1 = ptvCtrlPts[0].y;
		}
		ptvEvaluatedCurvePts.push_back(Point(x, y1));
		float y2;
		x = fAniLength;
		if (bWrap)
			y2 = y1;
		else
			y2 = ptvCtrlPts[iCtrlPtCount - 1].y;

		ptvEvaluatedCurvePts.push_back(Point(x, y2));
	}


	

}