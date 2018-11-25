#include "Collision.h"


bool Collision::Check_Collision(const Triangle& tri, const ML::Vec3& p) const
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
	return check >= this->judge;
}

//マス別に呼ばれる関数
bool Collision::Hit_Check(std::vector<After_Collision>* result, const std::vector<Triangle>& all_Tri, const ML::Vec3 box_Center, const float& box_Length, std::vector<ML::Vec3>& all_Points, const ML::Vec3& ball_Pos, const float& r, const ML::Vec3& speed)
{		
	//コンストラクタによってゼロベクトルとfalseで生成される
	After_Collision collision_True;


	//以前に判断したことに対して処理を別にする
	//以前の判定で衝突が起こる可能性があったので精密判定をする
	if (this->pricision_Flag)
	{	
		//衝突判定スタート
		for (const auto& tri : all_Tri)
		{			
			//マスとマス接触面でおかしい加速を防ぐ
			//移動ベクトルと衝突した三角形の法線ベクトルのcos値
			float cosSN;
			MyMath::Vector_Dot(&cosSN, speed, tri.normal);
			//cos値が1ということは内角が0度だということ、つまり物理的にあり得ない衝突
			//もしものために誤差範囲まで確認
			if (cosSN >= this->judge)
			{
				//なので無視する
				continue;
			}

			collision_True = After_Collision();
			for (const auto& p : all_Points)
			{
				//それ以外の場合であたり判定を行う
				if (this->Check_Collision(tri, p))
				{
					//以下あたった三角形の法線ベクトルとフラグを返す処理
					collision_True.collision_Flag = true;
					collision_True.normal = tri.normal;
					//もう登録されているものなのか確認する
					if (result->size() == 0 || collision_True != result->at(result->size() - 1))
					{
						result->push_back(collision_True);
					}
					//ポリゴン1個あたり1つの点の衝突が起きたらそれで次のポリゴンの判定をする
					break;
				}
			}
		}
	}	
	
	//精密判定は現在位置で行って精密判定フラグは移動したと予想されるところで判定をする
	ML::Vec3 next_Pos = ball_Pos + speed;
	//次の位置がボックスの対角線を半直径とする球とあったていると精密判定フラグを立てる
	this->pricision_Flag = ML::Vec3(next_Pos - box_Center).Length() <= r + abs( ROOT3 / 2.0f*(box_Length));
	
	
	if (collision_True.collision_Flag)
	{
		return true;
	}
	//あたらなかったらfalseを返す	
	return false;
}

