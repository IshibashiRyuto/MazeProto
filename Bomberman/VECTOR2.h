#pragma once
class VECTOR2
{
public:
	VECTOR2();
	VECTOR2(int x, int y);

	~VECTOR2();

	int x;
	int y;
	//代入演算子
	VECTOR2& operator = (const VECTOR2& vec);

	//添え字演算子
	int& operator[](int i);

	//比較演算子
	bool operator==(const VECTOR2& vec) const;
	bool operator!=(const VECTOR2& vec) const;

	//単項演算子
	VECTOR2& operator+=(const VECTOR2& vec);
	VECTOR2& operator-=(const VECTOR2& vec);
	VECTOR2& operator*=(int k);
	VECTOR2& operator/=(int k);
	VECTOR2 operator+()const;
	VECTOR2 operator-()const;
};

//ベクトルとベクトルの演算
//Vector2+Vector2
VECTOR2 operator+(const VECTOR2& u, const VECTOR2& v);
//Vector2-Vector2
VECTOR2 operator-(const VECTOR2& u, const VECTOR2& v);
//int*Vector2
VECTOR2 operator*(int k, const  VECTOR2& v);
//Vector2*int
VECTOR2 operator*(const VECTOR2& v, int k);
//Vector2/int
VECTOR2 operator/(const VECTOR2& v, int k);
