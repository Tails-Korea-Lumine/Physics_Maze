#include "Collision.h"

std::vector<Triangle> Collision::Get_Triangle_Box3D(ML::Box3D box, ML::QT rotation)
{
	//三角形ごときに保存する場所
	std::vector<Triangle> tri;

	//6面体には12個の三角形がある
	Triangle t[12] = {};
	
	//各頂点を示すインデックス
	ML::Vec3 vertex_Index[8] =
	{
		ML::Vec3(box.x,box.y,box.z),//0
		ML::Vec3(box.x,box.y,box.z + box.d),//1
		ML::Vec3(box.x + box.w,box.y,box.z + box.d),//2
		ML::Vec3(box.x + box.w,box.y,box.z),//3
		ML::Vec3(box.x,box.y + box.h,box.z),//4
		ML::Vec3(box.x,box.y + box.h,box.z + box.d),//5
		ML::Vec3(box.x + box.w,box.y + box.h,box.z + box.d),//6
		ML::Vec3(box.x + box.w,box.y + box.h,box.z)//7
	};

	//各頂点をワールド回転量で回転させる
	ML::Mat4x4 matR;
	//回転行列を作る
	matR.RotationQuaternion(rotation);
	
	for (int i = 0; i < 8; i++)
	{
		vertex_Index[i] = matR.TransformCoord(vertex_Index[i]);
	}


	//三角形の情報入力
	t[0] =
	{
		t[0].a = vertex_Index[0],
		t[0].b = vertex_Index[3],
		t[0].c = vertex_Index[4],
		t[0].normal = Gravity::Get_Normal_to_Vector_Cross((t[0].c - t[0].a), (t[0].b - t[0].a))
	};
	t[1] =
	{
		t[1].a = vertex_Index[3],
		t[1].b = vertex_Index[4],
		t[1].c = vertex_Index[7],
		t[1].normal = Gravity::Get_Normal_to_Vector_Cross((t[1].b - t[1].a), (t[1].c - t[1].a))
	};
	t[2] =
	{
		t[2].a = vertex_Index[1],
		t[2].b = vertex_Index[0],		
		t[2].c = vertex_Index[5],
		t[2].normal = Gravity::Get_Normal_to_Vector_Cross((t[2].c - t[2].a), (t[2].b - t[2].a))
	};
	t[3] =
	{
		t[3].a = vertex_Index[0],
		t[3].b = vertex_Index[5],
		t[3].c = vertex_Index[4],
		t[3].normal = Gravity::Get_Normal_to_Vector_Cross((t[3].b - t[3].a), (t[3].c - t[3].a))
	};
	t[4] =
	{
		t[4].a = vertex_Index[1],
		t[4].b = vertex_Index[5],
		t[4].c = vertex_Index[2],
		t[4].normal = Gravity::Get_Normal_to_Vector_Cross((t[4].c - t[4].a), (t[4].b - t[4].a))
	};
	t[5] =
	{
		t[5].a = vertex_Index[5],
		t[5].b = vertex_Index[2],
		t[5].c = vertex_Index[6],
		t[5].normal = Gravity::Get_Normal_to_Vector_Cross((t[5].b - t[5].a), (t[5].c - t[5].a))
	};
	t[6] =
	{
		t[6].a = vertex_Index[3],
		t[6].b = vertex_Index[7],
		t[6].c = vertex_Index[2],
		t[6].normal = Gravity::Get_Normal_to_Vector_Cross((t[6].b - t[6].a),(t[4].c - t[6].a))
	};
	t[7] =
	{
		t[7].a = vertex_Index[7],
		t[7].b = vertex_Index[2],
		t[7].c = vertex_Index[6],
		t[7].normal = Gravity::Get_Normal_to_Vector_Cross((t[7].c - t[7].a), (t[7].b - t[7].a))
	};
	t[8] =
	{
		t[8].a = vertex_Index[4],
		t[8].b = vertex_Index[7],
		t[8].c = vertex_Index[5],
		t[8].normal = Gravity::Get_Normal_to_Vector_Cross((t[8].c - t[8].a), (t[8].b - t[8].a))
	};
	t[9] =
	{
		t[9].a = vertex_Index[7],
		t[9].b = vertex_Index[5],
		t[9].c = vertex_Index[6],
		t[9].normal = Gravity::Get_Normal_to_Vector_Cross((t[9].b - t[9].a),(t[9].c - t[9].a))

	};
	t[10] =
	{
		t[10].a = vertex_Index[0],
		t[10].b = vertex_Index[3],
		t[10].c = vertex_Index[1],
		t[10].normal = Gravity::Get_Normal_to_Vector_Cross((t[10].b - t[10].a),(t[10].c - t[10].a))
	};
	t[11] =
	{
		t[11].a = vertex_Index[3],
		t[11].b = vertex_Index[1],
		t[11].c = vertex_Index[2],
		t[11].normal = Gravity::Get_Normal_to_Vector_Cross((t[11].c - t[11].a), (t[11].b - t[11].a))
	};

	//std::vectorでまとめた後に
	for (int i = 0; i < 12; i++)
	{
		t[i].normal.Normalize();
		tri.push_back(t[i]);
	}
	//戻り値で返す
	return tri;
}

bool Collision::Check_Collision(Triangle tri, ML::Vec3 p)
{
	//とある点pと三角形の当たり判定
	//pから三角形の各頂点へのベクトルA,B,C
	//そのA,B,Cが構成する角度の和が360	だったら
	//点ｐは三角形の辺の上または内包している。

	ML::Vec3 A, B, C;
	A = tri.a - p;
	B = tri.b - p;
	C = tri.c - p;

	//alpha ＝ AとBの角度
	//beta = BとCの角度
	//gamma = AとCの角度
	float cosAlpha;
	float cosBeta;
	float cosGamma;
	float sinAlpha;
	float sinBeta;
	float sinGamma;

	cosAlpha = Collision::Vector_Dot(A, B);
	cosBeta = Collision::Vector_Dot(B, C);
	cosGamma = Collision::Vector_Dot(A, C);
	sinAlpha = Collision::Vector_Cross(A, B);
	sinBeta = Collision::Vector_Cross(B, C);
	sinGamma = Collision::Vector_Cross(A, C);

	float check;

	//cos(alpha + beta + gamma)の展開式
	check = (cosGamma*((cosAlpha * cosBeta) - (sinAlpha * sinBeta))) - (sinGamma * ((cosAlpha*sinBeta) + (sinAlpha*cosBeta)));

	//誤差まで確認(誤差の範囲は変わる余地がある 2018/03/16)
	//0.99756405026
	if (check  >= _CMATH_::cos(ML::ToRadian(358)))
	{
		return true;
	}

	return false;
}

std::vector<ML::Vec3> Collision::Get_6Poiont_to_Sphere(ML::Vec3 pos, float r, ML::QT rotation)
{
	std::vector<ML::Vec3> S;
	ML::Vec3 v[6] = {};

	ML::Mat4x4 matR;

	matR.RotationQuaternion(rotation);

	//6個だけをとるver.1
	/*v[0] = pos + ML::Vec3(+r, 0, 0);
	v[1] = pos + ML::Vec3(-r, 0, 0);
	v[2] = pos + ML::Vec3(0, +r, 0);
	v[3] = pos + ML::Vec3(0, -r, 0);
	v[4] = pos + ML::Vec3(0, 0, +r);
	v[5] = pos + ML::Vec3(0, 0, -r);

	for (int i = 0; i < 6; i++)
	{
		v[i] = matR.TransformCoord(v[i]);
		S.push_back(v[i]);
	}*/
	//半直径の半分の範囲までとるver.2
	for (int i = r; i > r/2; i--)
	{
		v[0] = pos + ML::Vec3(+i, 0, 0);
		v[1] = pos + ML::Vec3(-i, 0, 0);
		v[2] = pos + ML::Vec3(0, +i, 0);
		v[3] = pos + ML::Vec3(0, -i, 0);
		v[4] = pos + ML::Vec3(0, 0, +i);
		v[5] = pos + ML::Vec3(0, 0, -i);

		for (int i = 0; i < 6; i++)
		{
			v[i] = matR.TransformCoord(v[i]);
			S.push_back(v[i]);
		}
	}
	return S;
}

//マス別に呼ばれる関数
After_Collision Collision::Hit_Check(ML::Box3D box, ML::Vec3 pos, float r, ML::QT worldR)
{
	//球の6個の頂点座標
	std::vector<ML::Vec3> sp = Collision::Get_6Poiont_to_Sphere(pos, r, worldR);
	//一個のマスにある12個の三角形
	std::vector<Triangle> all_Tri;
	all_Tri = Collision::Get_Triangle_Box3D(box, worldR);

	bool collision_Flag;

	//衝突判定スタート
	for (auto tri : all_Tri)
	{
		for (auto p : sp)
		{
			if (collision_Flag = Collision::Check_Collision(tri, p))
			{
				After_Collision collision_True;
				collision_True.collision_Flag = collision_Flag;
				collision_True.normal = tri.normal.Normalize();
				return collision_True;
			}
		}
	}

	After_Collision collision_False;
	collision_False.collision_Flag = false;
	collision_False.normal = ML::Vec3(0, 0, 0);
	return collision_False;
}