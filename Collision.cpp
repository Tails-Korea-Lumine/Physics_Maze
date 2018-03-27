#include "Collision.h"

std::vector<Triangle> Collision::Get_Triangle_Box3D(ML::Box3D box)
{
	//三角形ごときに保存する場所
	std::vector<Triangle> tri;

	//6面体には12個の三角形がある
	Triangle t[12] = {};
	
	//各三角形の指定(for文でできるかな 2018/03/16)
	t[0] =
	{
		t[0].a = ML::Vec3(box.x,box.y,box.z),
		t[0].b = ML::Vec3(box.x + box.w,box.y,box.z),
		t[0].c = ML::Vec3(box.x,box.y + box.h,box.z),
		t[0].normal = Gravity::Get_Normal_to_Vector_Cross((t[0].c - t[0].a), (t[0].b - t[0].a))
	};
	t[1] =
	{
		t[1].a = ML::Vec3(box.x + box.w,box.y,box.z),
		t[1].b = ML::Vec3(box.x,box.y + box.h,box.z),
		t[1].c = ML::Vec3(box.x + box.w,box.y + box.h,box.z),
		t[1].normal = Gravity::Get_Normal_to_Vector_Cross((t[1].c - t[1].a), (t[1].b - t[1].a))
	};
	t[2] =
	{
		t[2].a = ML::Vec3(box.x,box.y,box.z + box.d),
		t[2].b = ML::Vec3(box.x,box.y,box.z),		
		t[2].c = ML::Vec3(box.x,box.y + box.h,box.z+box.d),
		t[2].normal = Gravity::Get_Normal_to_Vector_Cross((t[2].c - t[2].a), (t[2].b - t[2].a))
	};
	t[3] =
	{
		t[3].a = ML::Vec3(box.x,box.y,box.z),
		t[3].b = ML::Vec3(box.x,box.y + box.h,box.z + box.d),
		t[3].c = ML::Vec3(box.x,box.y + box.h,box.z),
		t[3].normal = Gravity::Get_Normal_to_Vector_Cross((t[3].c - t[3].a), (t[3].b - t[3].a))
	};
	t[4] =
	{
		t[4].a = ML::Vec3(box.x,box.y,box.z + box.d),
		t[4].b = ML::Vec3(box.x,box.y + box.h,box.z + box.d),
		t[4].c = ML::Vec3(box.x + box.w,box.y,box.z + box.d),
		t[4].normal = Gravity::Get_Normal_to_Vector_Cross((t[4].c - t[4].a), (t[4].b - t[4].a))
	};
	t[5] =
	{
		t[5].a = ML::Vec3(box.x,box.y + box.h,box.z + box.d),
		t[5].b = ML::Vec3(box.x + box.w,box.y,box.z + box.d),
		t[5].c = ML::Vec3(box.x + box.w,box.y + box.h,box.z + box.d),
		t[5].normal = Gravity::Get_Normal_to_Vector_Cross((t[5].c - t[5].a), (t[5].b - t[5].a))
	};
	t[6] =
	{
		t[6].a = ML::Vec3(box.x + box.w,box.y,box.z),
		t[6].b = ML::Vec3(box.x + box.w,box.y + box.h,box.z),
		t[6].c = ML::Vec3(box.x + box.w,box.y,box.z + box.d),
		t[6].normal = Gravity::Get_Normal_to_Vector_Cross((t[6].b - t[6].a),(t[4].c - t[6].a))
	};
	t[7] =
	{
		t[7].a = ML::Vec3(box.x + box.w,box.y + box.h,box.z),
		t[7].b = ML::Vec3(box.x + box.w,box.y,box.z + box.d),
		t[7].c = ML::Vec3(box.x + box.w,box.y + box.h,box.z + box.d),
		t[7].normal = Gravity::Get_Normal_to_Vector_Cross((t[7].c - t[7].a), (t[7].b - t[7].a))
	};
	t[8] =
	{
		t[8].a = ML::Vec3(box.x,box.y + box.h,box.z),
		t[8].b = ML::Vec3(box.x + box.w,box.y + box.h,box.z),
		t[8].c = ML::Vec3(box.x,box.y + box.h,box.z + box.d),
		t[8].normal = Gravity::Get_Normal_to_Vector_Cross((t[8].c - t[8].a), (t[8].b - t[8].a))
	};
	t[9] =
	{
		t[9].a = ML::Vec3(box.x + box.w,box.y + box.h,box.z),
		t[9].b = ML::Vec3(box.x,box.y + box.h,box.z + box.d),
		t[9].c = ML::Vec3(box.x + box.w,box.y + box.h,box.z + box.d),
		t[9].normal = Gravity::Get_Normal_to_Vector_Cross((t[9].b - t[9].a),(t[9].c - t[9].a))

	};
	t[10] =
	{
		t[10].a = ML::Vec3(box.x,box.y,box.z),
		t[10].b = ML::Vec3(box.x + box.w,box.y,box.z),
		t[10].c = ML::Vec3(box.x,box.y,box.z + box.d),
		t[10].normal = Gravity::Get_Normal_to_Vector_Cross((t[10].b - t[10].a),(t[10].c - t[10].a))
	};
	t[11] =
	{
		t[11].a = ML::Vec3(box.x + box.w,box.y,box.z),
		t[11].b = ML::Vec3(box.x,box.y,box.z + box.d),
		t[11].c = ML::Vec3(box.x + box.w,box.y,box.z + box.d),
		t[11].normal = Gravity::Get_Normal_to_Vector_Cross((t[11].c - t[11].a), (t[11].b - t[11].a))
	};


	for (int i = 0; i < 12; i++)
	{
		t[i].normal.Normalize();
		tri.push_back(t[i]);
	}

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
	float cosAlpha, cosBeta, cosGamma;
	float sinAlpha, sinBeta, sinGamma;

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
	if (check  == 1)
	{
		return true;
	}

	return false;
}

std::vector<ML::Vec3> Collision::Get_6Poiont_to_Sphere(ML::Vec3 pos, float r)
{
	std::vector<ML::Vec3> S;
	ML::Vec3 v[6] = {};

	v[0] = pos + ML::Vec3(+r, 0, 0);
	v[1] = pos + ML::Vec3(-r, 0, 0);
	v[2] = pos + ML::Vec3(0, +r, 0);
	v[3] = pos + ML::Vec3(0, -r, 0);
	v[4] = pos + ML::Vec3(0, 0, +r);
	v[5] = pos + ML::Vec3(0, 0, -r);

	for (int i = 0; i < 6; i++)
	{
		S.push_back(v[i]);
	}

	return S;
}

//マス別に呼ばれる関数
After_Collision Collision::Hit_Check(ML::Box3D box, ML::Vec3 pos, float r)
{
	//球の6個の頂点座標
	std::vector<ML::Vec3> sp = Collision::Get_6Poiont_to_Sphere(pos, r);
	//一個のマスにある12個の三角形
	std::vector<Triangle> all_Tri;
	all_Tri = Collision::Get_Triangle_Box3D(box);

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