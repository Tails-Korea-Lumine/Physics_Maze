#include "Collision.h"
#include "Sphere.h"
#include "Cube.h"


bool Collision::Check_Collision_Triangle_Point(const Triangle& tri, const ML::Vec3& p)
{
	//とある点pと三角形の当たり判定
	//pから三角形の各頂点へのベクトルA,B,C
	//そのA,B,Cが構成する角度の和が360	だったら
	//点ｐは三角形の辺の上または内包している。

	ML::Vec3 A, B, C;
	A = tri.a - p;
	B = tri.b - p;
	C = tri.c - p;

	//三角形の頂点と同じくなった場合
	//if (A.Length() == 0.0f || B.Length() == 0.0f || C.Length() == 0.0f)
	if(A.Is_Zero_Vec() || B.Is_Zero_Vec() || C.Is_Zero_Vec())
	{
		return true; //計算不可能かつ三角形に内包されているのであたってる判定で返す
	}	
	

	//alpha ＝ AとBの角度
	//beta = BとCの角度
	//gamma = AとCの角度
	float cosAlpha;
	float cosBeta;
	float cosGamma;
	float sinAlpha;
	float sinBeta;
	float sinGamma;

	MyMath::Vector_Dot(&cosAlpha,A, B);
	MyMath::Vector_Dot(&cosBeta, B, C);
	MyMath::Vector_Dot(&cosGamma, A, C);
	MyMath::Vector_Cross(&sinAlpha, A, B);
	MyMath::Vector_Cross(&sinBeta, B, C);
	MyMath::Vector_Cross(&sinGamma, A, C);

	float check;

	//cos(alpha + beta + gamma)の展開式
	check = (cosGamma*((cosAlpha * cosBeta) - (sinAlpha * sinBeta))) - (sinGamma * ((cosAlpha*sinBeta) + (sinAlpha*cosBeta)));

	//誤差まで確認(誤差の範囲は変わる余地がある 2018/03/16)
	//0.99756405026
	return check >= judge;
}

//OBBをAABBに戻して判定
void Collision::Intersect_OBB_Sphere(ML::Vec3* result, const Shape3D* owner, const Shape3D* visitor)
{
	//今までの回転量で逆行列を作る
	ML::Mat4x4 matR, matIR;
	//キューブの情報をもらっておく
	ML::Vec3 o_Center = owner->Get_Center();
	ML::Vec3 o_Halfl = owner->Get_Length();

	D3DXMatrixAffineTransformation(&matR, 1.0f, &owner->Get_Center(), &owner->Get_Quaternion(), NULL);
	matIR = matR.Inverse();

	//逆行列に沿って回転した球の中心点
	ML::Vec3 alined_Sphere_Center = matIR.TransformCoord(visitor->Get_Center());

	//一番近い点を計算
	float x = max(o_Center.x - o_Halfl.x, min(alined_Sphere_Center.x, o_Center.x + o_Halfl.x));					  
	float y = max(o_Center.y - o_Halfl.y, min(alined_Sphere_Center.y, o_Center.y + o_Halfl.y));					  
	float z = max(o_Center.z - o_Halfl.z, min(alined_Sphere_Center.z, o_Center.z + o_Halfl.z));
	
	//相対距離ベクトルをもらって
	ML::Vec3 dist = ML::Vec3(x, y, z) - alined_Sphere_Center;
	//相対ベクトルの長さによって近い点を計算
	*result = dist.Length() <= visitor->Get_Length().x ? ML::Vec3(x, y, z) : alined_Sphere_Center + dist.Normalize() * -visitor->Get_Length().x;
	//回転を元に戻して処理終了
	*result = matR.TransformCoord(*result);
}


//マス別に呼ばれる関数
bool Collision::Check_Collision_Cube_Sphere(std::vector<Collision_Data>* result, const Shape3D* owner, const ML::Vec3& nearest_point)
{		
	//コンストラクタによってゼロベクトルとfalseで生成される
	Collision_Data cd;


	//判定用の三角形を取り出す
	std::vector<Triangle> all_Tri;
	owner->Get_Triangle_Box3D(&all_Tri);

	for (auto& tri : all_Tri)
	{
		//例外でないかつ三角形と点のあたり判定が合ってる場合で保存する
		if (Check_Collision_Triangle_Point(tri, nearest_point))
		{
			//以下あたった三角形の法線ベクトルとフラグを返す処理
			cd.collision_Flag = true;
			cd.normal = tri.normal;
			//もう登録されているものなのか確認する
			if (result->size() == 0 || cd != result->at(result->size() - 1))
			{
				result->push_back(cd);
			}			
		}
	}

	return cd.collision_Flag;
}