#include "Collision.h"
#include "MyMath.h"

#define TRIANGLE_ON_CUBE 12
#define VERTEX_ON_CUBE 8
#define ROOT3 1.732050f

void Collision::Get_Triangle_Box3D(std::vector<Triangle>* result, const ML::Box3D& box, const ML::QT& rotation) const
{
	//三角形ごときに保存する場所
	//std::vector<Triangle> tri;

	//6面体には12個の三角形がある
	Triangle t[TRIANGLE_ON_CUBE] = {};
	
	//各頂点を示すインデックス
	ML::Vec3 vertex_Index[VERTEX_ON_CUBE] =
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
	//matR.RotationQuaternion(rotation);
	ML::Vec3 center = { box.x + (box.w / 2), box.y + (box.h / 2), box.z + (box.d / 2) };
	D3DXMatrixAffineTransformation(&matR, 1, &center, &rotation, NULL);
	
	for (int i = 0; i < VERTEX_ON_CUBE; i++)
	{
		vertex_Index[i] = matR.TransformCoord(vertex_Index[i]);
	}


	//三角形の情報入力
	t[0] =
	{
		vertex_Index[0],
		vertex_Index[3],
		vertex_Index[4],		
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[0].normal, (t[0].c - t[0].a), (t[0].b - t[0].a));
	t[1] =
	{
		vertex_Index[3],
		vertex_Index[4],
		vertex_Index[7],		
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[1].normal, (t[1].b - t[1].a), (t[1].c - t[1].a));
	t[2] =
	{
		vertex_Index[1],
		vertex_Index[0],
		vertex_Index[5],		
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[2].normal, (t[2].c - t[2].a), (t[2].b - t[2].a));
	t[3] =
	{
		vertex_Index[0],
		vertex_Index[5],
		vertex_Index[4],		
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[3].normal, (t[3].b - t[3].a), (t[3].c - t[3].a));
	t[4] =
	{
		vertex_Index[1],
		vertex_Index[5],
		vertex_Index[2],
		
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[4].normal, (t[4].c - t[4].a), (t[4].b - t[4].a));
	t[5] =
	{
		vertex_Index[5],
		vertex_Index[2],
		vertex_Index[6],
		
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[5].normal, (t[5].b - t[5].a), (t[5].c - t[5].a));
	t[6] =
	{
		vertex_Index[3],
		vertex_Index[7],
		vertex_Index[2],
		
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[6].normal, (t[6].b - t[6].a), (t[4].c - t[6].a));
	t[7] =
	{
		vertex_Index[7],
		vertex_Index[2],
		vertex_Index[6],
		
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[7].normal, (t[7].c - t[7].a), (t[7].b - t[7].a));
	t[8] =
	{
		vertex_Index[4],
		vertex_Index[7],
		vertex_Index[5],
		
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[8].normal, (t[8].c - t[8].a), (t[8].b - t[8].a));
	t[9] =
	{
		vertex_Index[7],
		vertex_Index[5],
		vertex_Index[6],
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[9].normal, (t[9].b - t[9].a), (t[9].c - t[9].a));
	t[10] =
	{
		vertex_Index[0],
		vertex_Index[3],
		vertex_Index[1],
		
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[10].normal, (t[10].b - t[10].a), (t[10].c - t[10].a));
	t[11] =
	{
		vertex_Index[3],
		vertex_Index[1],
		vertex_Index[2],		
	};
	MyMath::Get_Normal_to_Vector_Cross(&t[11].normal, (t[11].c - t[11].a), (t[11].b - t[11].a));

	//std::vector resultに登録させる
	for (int i = 0; i < TRIANGLE_ON_CUBE; i++)
	{
		t[i].normal = t[i].normal.Normalize();
		result->push_back(t[i]);
	}
}

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
	if (check  >= this->judge)
	{
		return true;
	}

	return false;
}

void Collision::Get_Poionts_to_Sphere(std::vector<ML::Vec3>* result ,const ML::Vec3& pos, const float& r) const
{
	const int increasing_Dgree = 10;
	//球の上にある点を全部取り出す処理

	//最初に回転させる点を計算
	std::vector<ML::Vec3> points;
	ML::Vec3 y = pos + ML::Vec3(0, r, 0);

	//縦方向に切った断面の片方を取る処理
	for (int i = 0; i < 180; i += increasing_Dgree)
	{
		//回転行列生成
		ML::Mat4x4 matRx;
		ML::QT qtX = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian((float)i));

		D3DXMatrixAffineTransformation(&matRx, 1.0f, &pos, &qtX, NULL);

		points.push_back(matRx.TransformCoord(y));
	}
	
	//取り出した点を回転しながら結果保存用ヴェクターにプッシュバック
	for (int d = 0; d < 360; d += increasing_Dgree)
	{
		//回転行列生成
		ML::Mat4x4 matRy;
		ML::QT qtY = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian((float)d));

		D3DXMatrixAffineTransformation(&matRy, 1.0f, &pos, &qtY, NULL);
		
		for (auto& p : points)
		{
			result->push_back(matRy.TransformCoord(p));
		}
	}
}

//マス別に呼ばれる関数
bool Collision::Hit_Check(std::vector<After_Collision>* result, const ML::Box3D& box, const ML::Vec3& pos, const float& r, const ML::Vec3& speed, const ML::QT& Rotation)
{	
	//一個のマスにある12個の三角形
	std::vector<Triangle> all_Tri;
	this->Get_Triangle_Box3D(&all_Tri, box, Rotation);

	std::vector<ML::Vec3> all_Points;
	//最短距離の座標も追加
	this->Get_ShortisetPoints_Box_to_Sphere(&all_Points, box);	
	//球の頂点座標
	this->Get_Poionts_to_Sphere(&all_Points, pos, r);
	//コンストラクタによってゼロベクトルとfalseで生成される
	After_Collision collision_True;

	//以前に判断したことに対して処理を別にする
	//以前の判定で衝突が起こる可能性があったので精密判定をする
	if (this->pricision_Flag)
	{
		//衝突判定スタート
		for (const auto& tri : all_Tri)
		{
			collision_True = After_Collision();
			for (const auto& p : all_Points)
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

				//それ以外の場合であたり判定を行う
				if (this->Check_Collision(tri, p))
				{
					//以下あたった三角形の法線ベクトルとフラグを返す処理
					collision_True.collision_Flag = true;
					collision_True.normal = tri.normal;
					result->push_back(collision_True);
					//ポリゴン1個あたり1つの点の衝突が起きたらそれで次のポリゴンの判定をする
					break;
				}
			}
		}
	}	
	
	//精密判定は現在位置で行って精密判定フラグは移動したと予想されるところで判定をする
	ML::Vec3 next_Pos = pos + speed;
	ML::Vec3 box_Center = { box.x + (box.w / 2), box.y + (box.h / 2), box.z + (box.d / 2) };
	//次の位置がボックスの対角線を半直径とする球とあったていると精密判定フラグを立てる
	ML::Vec3(next_Pos - box_Center).Length() <= r + ROOT3 / 2.0f*(box.x) ? this->pricision_Flag = true : this->pricision_Flag = false;	
	
	
	if (collision_True.collision_Flag)
	{
		return true;
	}
	//あたらなかったらfalseを返す	
	return false;
}

//Box3dと球体の最短距離の点を取る
void Collision::Get_ShortisetPoints_Box_to_Sphere(std::vector<ML::Vec3>* result, const ML::Box3D& box) const
{
	//ver1.0
	////ボックスの中心を確保する
	//ML::Vec3 center_of_Box = ML::Vec3(box.x + box.w / 2, box.y + box.h / 2, box.z + box.d / 2);
	////二つの中心の相対距離を算出
	//ML::Vec3 relative_Distance = center_of_Box - pos;

	////相対距離を方向性だけを残すために正規化する
	//relative_Distance = relative_Distance.Normalize();

	//
	////最短距離が出る方向のベクトルに半直径を掛けて
	////最短距離の点を返す
	//for (float d = r + 2; d > r - 2; d --)
	//{
	//	result->push_back(pos + (relative_Distance * d));
	//}

	

	//ver2.0
	//引数でもらったボックスに外接する球に内包する点だけを残す	
	//外接する球の半直径
	float outer_Sphere_R = ROOT3 / 2.0f*(box.x);
	//外接する球の中心点
	ML::Vec3 box_Center = { box.x + (box.w / 2), box.y + (box.h / 2), box.z + (box.d / 2) };

	result->erase(remove_if(result->begin(), result->end(),
							[&](ML::Vec3 p)->bool{return ML::Vec3(p - box_Center).Length() <= outer_Sphere_R;}
						),
		result->end()
	);
	
}