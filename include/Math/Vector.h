#pragma once

#include <iostream>
#include <limits.h>
#include <cmath>
#include <vector>

/////////////////////////////////////////////////////////////////////////////////
//Vector2D
/////////////////////////////////////////////////////////////////////////////////
class Vector2f
{
    public:
        float x;
        float y;

        Vector2f();

        Vector2f(float x, float y) : x(x), y(y) {};
        Vector2f(float xy) : x(xy), y(xy) {};

        void set(float x, float y);
		float* get() { float arr[] = { x, y }; return arr; };

        Vector2f operator= (const Vector2f vec);
        Vector2f operator+ (const Vector2f vec)const;
        Vector2f operator- (const Vector2f vec)const;
        Vector2f operator+= (const Vector2f vec);
        Vector2f operator-= (const Vector2f vec);
        Vector2f operator/ (const Vector2f vec)const;
        Vector2f operator/ (const float& num)const;
        Vector2f operator* (const Vector2f vec)const;
        Vector2f operator*= (const Vector2f vec);
        Vector2f operator* (const float num)const;
        Vector2f operator*= (const float num);
        bool operator!= (const Vector2f vec);

    protected:
    private:
};

std::ostream& operator<< (std::ostream& output, const Vector2f& vec);

class Vector2i
{
    public:
        int x;
        int y;

        Vector2i();

        Vector2i(int x, int y) : x(x), y(y) {};
        Vector2i(int xy) : x(xy), y(xy) {};

        void set(int x, int y);
		int* get() { int arr[] = { x, y }; return arr; };
		Vector2f getVec2f() { return Vector2f(x, y); }

        Vector2i operator= (const Vector2i vec);
        Vector2i operator+ (const Vector2i vec)const;
        Vector2i operator- (const Vector2i vec)const;
        Vector2i operator/ (const Vector2i vec)const;
        Vector2i operator/ (const int& num)const;
        Vector2i operator* (const Vector2i vec)const;
        Vector2i operator* (const int& num)const;
        bool operator== (const Vector2i& vec);
        bool operator!= (const Vector2i& vec);


    protected:
    private:
};

std::ostream& operator<< (std::ostream& output, const Vector2i& vec);

/////////////////////////////////////////////////////////////////////////////////
//Vector3D
/////////////////////////////////////////////////////////////////////////////////
class Vector3f
{
    public:
        float x;
        float y;
        float z;

        Vector3f();
        Vector3f(float xyz) : x(xyz), y(xyz), z(xyz) {};

        Vector3f(float x, float y, float z) : x(x), y(y), z(z) {};
        Vector3f&    normal();
        Vector3f    normalize(Vector3f vec3);
        bool        equal(const Vector3f& vec, float e) const;

        void set(float x, float y, float z);
		float* get() { float arr[] = { x, y, z }; return arr; };

		Vector2f getVector2f() { return Vector2f(x, y); }

        Vector3f operator= (const Vector3f vec);
        Vector3f operator+ (const Vector3f vec)const;
        Vector3f operator- (const Vector3f vec)const;
        Vector3f operator/ (const Vector3f vec)const;
        Vector3f operator/ (const float& num)const;
        Vector3f operator* (const Vector3f vec)const;
        Vector3f operator* (const float& num)const;
        void operator*= (const Vector3f vec);
        Vector3f operator+= (const Vector3f& vec);
        Vector3f operator-= (const Vector3f& vec);
        float& operator []  (const unsigned int index);
        bool operator!= (const Vector3f vec);
        bool operator== (const Vector3f vec);

    protected:
    private:
};
std::ostream& operator<< (std::ostream& output, const Vector3f& vec);

class Vector3i
{
    public:
        int x;
        int y;
        int z;

        Vector3i();
        Vector3i(int xyz) : x(xyz), y(xyz), z(xyz) {};

        Vector3i(int x, int y, int z) : x(x), y(y), z(z) {};
        Vector3i&    normal();
        Vector3i    normalize(Vector3i vec3);
        bool        equal(const Vector3i& vec, float e) const;

        void set(int x, int y, int z);
		int* get() { int arr[] = { x, y, z }; return arr; };

        Vector3i operator= (const Vector3i& vec);
        Vector3i operator+ (const Vector3i& vec)const;
        Vector3i operator- (const Vector3i& vec)const;
        Vector3i operator/ (const Vector3i& vec)const;
        Vector3i operator/ (const float& num)const;
        Vector3i operator* (const Vector3i& vec)const;
        Vector3i operator* (const float& num)const;
        void operator*= (const Vector3i& vec);
        Vector3i operator+= (const Vector3i& vec);
        Vector3i operator-= (const Vector3i& vec);
        float& operator []  (const unsigned int index);
        bool operator!= (const Vector3i& vec);

    protected:
    private:
};
std::ostream& operator<< (std::ostream& output, const Vector3i& vec);

/////////////////////////////////////////////////////////////////////////////////
//Vector4D
/////////////////////////////////////////////////////////////////////////////////
class Vector4f
{
    public:
        float x;
        float y;
        float z;
        float w;

        Vector4f() : x(0), y(0), z(0), w(0) {};

        Vector4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

        void set(float x, float y, float z, float w);
		float* get() { float arr[] = { x, y, z, w }; return arr; };

        Vector4f operator= (const Vector4f& vec);
        Vector4f operator+ (const Vector4f& vec)const;
        Vector4f operator- (const Vector4f& vec)const;
        Vector4f operator/ (const Vector4f& vec)const;
        Vector4f operator/ (const float& num)const;
        Vector4f operator* (const Vector4f& vec)const;
        Vector4f operator* (const float& num)const;
        bool operator!= (const Vector4f& vec);

    protected:
    private:
};

std::ostream& operator<< (std::ostream& output, const Vector4f& vec);

class Vector4i
{
    public:
        int x;
        int y;
        int z;
        int w;

        Vector4i() : x(0), y(0), z(0), w(0) {};

        Vector4i(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {};

        void set(int x, int y, int z, int w);
		int* get() { int arr[] = { x, y, z, w }; return arr; };

        Vector4i operator= (const Vector4i& vec);
        Vector4i operator+ (const Vector4i& vec)const;
        Vector4i operator- (const Vector4i& vec)const;
        Vector4i operator/ (const Vector4i& vec)const;
        Vector4i operator/ (const float& num)const;
        Vector4i operator* (const Vector4i& vec)const;
        Vector4i operator* (const float& num)const;
        bool operator!= (const Vector4i& vec);

    protected:
    private:
};

std::ostream& operator<< (std::ostream& output, const Vector4i& vec);