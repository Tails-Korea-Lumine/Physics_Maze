#include "Collision.h"
#include "Sphere.h"
#include "Cube.h"

#define TRIANGLE_ON_CUBE 12


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
bool Collision::Intersect_AABB_Sphere(ML::Vec3* result, const Shape3D* owner, const Shape3D* visitor)
{
	//今までの回転量で逆行列を作る
	ML::Mat4x4 matIR;

	D3DXMatrixAffineTransformation(&matIR, 1.0f, &owner->Get_Center(), &owner->Get_Quaternion(), NULL);
	matIR = matIR.Inverse();


	//逆行列に沿って回転した球の中心点
	ML::Vec3 alined_Sphere_Center = matIR.TransformCoord(visitor->Get_Center());

	//一番近い点を計算
	float x = max(owner->center.x - owner->half_of_Length.x, min(alined_Sphere_Center.x, owner->center.x + owner->half_of_Length.x));
	float y = max(owner->center.y - owner->half_of_Length.y, min(alined_Sphere_Center.y, owner->center.y + owner->half_of_Length.y));
	float z = max(owner->center.z - owner->half_of_Length.z, min(alined_Sphere_Center.z, owner->center.z + owner->half_of_Length.z));

	//*result = ML::Vec3(x, y, z);

	//完璧に離れているならfalseを返す
	ML::Vec3 dist = ML::Vec3(x, y, z) - visitor->Get_Center();

	*result = dist.Length() < visitor->Get_Length().x ? ML::Vec3(x, y, z) : visitor->Get_Center() + dist.Normalize() * visitor->Get_Length().x;
	return true;
}


//マス別に呼ばれる関数
bool Collision::Check_Collision_Cube_Sphere(std::vector<Collision_Data>* result, const Shape3D* owner, const ML::Vec3& nearest_point, const bool unsuable_Triangle[])
{		
	//コンストラクタによってゼロベクトルとfalseで生成される
	Collision_Data collision_True;


	//以前に判断したことに対して処理を別にする
	//以前の判定で衝突が起こる可能性があったので精密判定をする
	
		////衝突判定スタート
		//for (const auto& tri : all_Tri)
		//{			
		//	//マスとマス接触面でおかしい加速を防ぐ
		//	//移動ベクトルと衝突した三角形の法線ベクトルのcos値
		//	float cosSN;
		//	MyMath::Vector_Dot(&cosSN, speed, tri.normal);
		//	//cos値が1ということは内角が0度だということ、つまり物理的にあり得ない衝突
		//	//もしものために誤差範囲まで確認
		//	if (cosSN >= judge)
		//	{
		//		//なので無視する
		//		continue;
		//	}

		//	collision_True = After_Collision();
		//	for (const auto& p : all_Points)
		//	{
		//		//それ以外の場合であたり判定を行う
		//		if (Collision::Check_Collision(tri, p))
		//		{
		//			//以下あたった三角形の法線ベクトルとフラグを返す処理
		//			collision_True.collision_Flag = true;
		//			collision_True.normal = tri.normal;
		//			//もう登録されているものなのか確認する
		//			if (result->size() == 0 || collision_True != result->at(result->size() - 1))
		//			{
		//				result->push_back(collision_True);
		//			}
		//			//ポリゴン1個あたり1つの点の衝突が起きたらそれで次のポリゴンの判定をする
		//			break;
		//		}
		//	}
		//}

	//判定用の三角形を取り出す
	std::vector<Triangle> all_Tri;
	owner->Get_Triangle_Box3D(&all_Tri);

	for (int i = 0; i < TRIANGLE_ON_CUBE; i++)
	{
		//例外でないかつ三角形と点のあたり判定が合ってる場合で保存する
		//unsuable_Triangle[i] == false &&
		if ( Check_Collision_Triangle_Point(all_Tri[i], nearest_point))
		{
			//以下あたった三角形の法線ベクトルとフラグを返す処理
			collision_True.collision_Flag = true;
			collision_True.normal = all_Tri[i].normal;
			//もう登録されているものなのか確認する
			if (result->size() == 0 || collision_True != result->at(result->size() - 1))
			{
				result->push_back(collision_True);
			}			
		}
	}
		
	
	//
	////精密判定は現在位置で行って精密判定フラグは移動したと予想されるところで判定をする
	//ML::Vec3 next_Pos = ball_Pos + speed;
	////次の位置がボックスの対角線を半直径とする球とあったていると精密判定フラグを立てる
	//this->pricision_Flag = ML::Vec3(next_Pos - box_Center).Length() <= r + abs( ROOT3 / 2.0f*(box_Length));
	
	return collision_True.collision_Flag;
}

