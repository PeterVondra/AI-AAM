#pragma once

#include "include/Math/Vector.h"

namespace xnn
{
	// LINEAR REGRESSION
	float pa(float b, std::vector<Vector2f>& v)
	{
		Vector2f ds(0);
		float sqsum = 0;

		for (auto& vec : v)
		{
			ds.x += vec.x;
			ds.y += vec.x * vec.y;
			sqsum += vec.x * vec.x;
		}

		return (b * ds.x - ds.y) / (-sqsum);
	}
	// LINEAR REGRESSION
	float pb(float a, std::vector<Vector2f>& v)
	{
		Vector2f ds(0);

		for (auto& vec : v)
			ds += vec;

		int d = v.size();

		return (ds.y - a * ds.x) / d;
	}

	// polynomial regression 2nd degree
	float pa2(float b, float c, std::vector<Vector2f>& v)
	{
		float xsqsum = 0;
		float xcusum = 0;
		float xfosum = 0;
		float xsqysum = 0;

		for (auto& vec : v)
		{
			xsqsum += vec.x * vec.x;
			xcusum += vec.x * vec.x * vec.x;
			xfosum -= vec.x * vec.x * vec.x * vec.x;
			xsqysum += vec.x * vec.x * vec.y;
		}

		return (b * xcusum + c * xsqsum - xsqysum) / xfosum;
	}
	float pb2(float a, float c, std::vector<Vector2f>& v)
	{
		float xsqsum = 0;
		float xcusum = 0;
		float xysum = 0;
		float xsum = 0;

		for (auto& vec : v)
		{
			xsqsum += vec.x * vec.x;
			xcusum += vec.x * vec.x * vec.x;
			xysum += vec.x * vec.y;
			xsum += vec.x;
		}

		return (-a * xcusum - c * xsum + xysum) / xsqsum;
	}
	float pc2(float a, float b, std::vector<Vector2f>& v)
	{
		float xsqsum = 0;
		float xsum = 0;
		float ysum = 0;

		for (auto& vec : v)
		{
			xsqsum += vec.x * vec.x;
			xsum += vec.x;
			ysum += vec.y;
		}

		return (ysum - a * xsqsum - b * xsum) / v.size();
	}

	void lineReg(float& a, float& b, unsigned int iterations, std::vector<Vector2f>& data)
	{
		a = 0;
		b = 0;

		for (int i = 0; i < iterations; i++)
		{
			a = pa(b, data);
			b = pb(a, data);
		}
	}

	void poly2Reg(float& a, float& b, float& c, unsigned int iterations, std::vector<Vector2f>& data)
	{
		a = 0;
		b = 0;

		for (int i = 0; i < iterations; i++)
		{
			a = pa2(b, c, data);
			b = pb2(a, c, data);
			c = pc2(a, b, data);
		}
	}
	// v == variables
	void polyReg(std::vector<float>& v, float lr, unsigned int iterations, std::vector<Vector2f>& data)
	{
		if (v.empty())
			return;
		std::vector<float> nsum(v.size());

		for (auto& var : v)
			var = 0.001;

		for (int i = 0; i < iterations; i++)
		{
			for (int j = 0; j < v.size(); j++)
			{
				for (auto& p : data)
				{
					float sum = 0;
					for (int c = 0; c < v.size(); c++)
						sum += v[c] * pow(p.x, v.size() - c - 1) - p.y;

					nsum[j] += 2 * pow(p.x, v.size() - j - 1) * sum;
				}

				v[j] = v[j] - lr * nsum[j];
			}
			nsum = std::vector<float>(v.size());
		}

		for (int i = 0; i < v.size(); i++) {
			if (v[i] == 0)
				continue;

			if (v[i] > 0 && i > 0)
				std::cout << "+";

			if(i < v.size() - 2)
				std::cout << v[i] << "x^" << v.size() - i - 1;
			else if(i == v.size() - 2)
				std::cout << v[i] << "x";
			else if(i < v.size() - 1)
				std::cout << v[i];
		}

		std::cout << std::endl;
	}
	void trigReg(float& a, float& b, float& c, float& d, float lr, unsigned int iterations, std::vector<Vector2f>& data)
	{
		a = 0.001;
		b = 0.001;
		c = 0.001;
		d = 0.001;
		float sum[4] = { 0, 0, 0, 0 };

		for (int i = 0; i < iterations; i++) {
			for (auto& p : data)
			{
				float sum_2 = (a * std::sin(p.x) + b * std::cos(p.x) + c * std::tan(p.x) + d - p.y);
				sum[0] += 2*std::sin(p.x)*sum_2;
				sum[1] += 2*std::cos(p.x)*sum_2;
				sum[2] += 2*std::tan(p.x)*sum_2;
				sum[3] += 2*sum_2;
			}

			a = a - lr * sum[0];
			b = b - lr * sum[1];
			c = c - lr * sum[2];
			d = d - lr * sum[3];

			sum[0] = 0;
			sum[1] = 0;
			sum[2] = 0;
			sum[3] = 0;
		}

		std::cout << a << "sin(x)" << " + " << b << "cos(x)" << " + " << c << "tan(x)" << " + " << d << std::endl;
	}
}