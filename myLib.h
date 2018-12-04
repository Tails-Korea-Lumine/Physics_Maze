#pragma warning(disable:4996)
#pragma once
#pragma warning( disable : 4005 )
#include <d3dx10.h>
#include <d3dCompiler.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma warning( default : 4005 )


#include <memory>
#include <tchar.h>
#include <windows.h>
#include <string.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
#include <unordered_map>
//////////////////////////////#include "atlstr.h"
using namespace std;
using namespace std::tr1;

namespace ML
{
	float Gravity(const  float  byM_);

	static const float  PI = 3.14159265358979323846f;
	float ToRadian(const  float	degree_);
	float ToDegree(const  float	radian_);

	//ベクトル型２Ｄ・３Ｄ・４Ｄ
	class Vec2 : public  D3DXVECTOR2
	{
	public:
		Vec2( );
		Vec2(const float  x_,const float  y_);
		Vec2(const D3DXVECTOR2&  v_);
		Vec2  operator  = (const D3DXVECTOR2&  v_);
		//ベクトルを正規化
		Vec2  Normalize(  )const;
		//ベクトルの長さを求める
		float  Length()const;
	};
	class Vec3 : public  D3DXVECTOR3
	{
	public:
		Vec3( );
		Vec3(const float  x_,const float  y_,const float  z_);
		Vec3(const D3DXVECTOR3&  v_);
		Vec3  operator  = (const D3DXVECTOR3&  v_);
		//ベクトル型変換
		class  Vec4  Vec4(  );
		//ベクトルを正規化
		Vec3  Normalize()const;
		//ベクトルの長さを求める
		float  Length()const;
		//ゼロベクトル(無効)なのかを確認する、trueだったらゼロベクトル
		bool Is_Zero_Vec() const;
	};
	class Vec4 : public  D3DXVECTOR4
	{
	public:
		Vec4( );
		Vec4(const float  x_,const float  y_,const float  z_,const float  w_);
		Vec4(const D3DXVECTOR4&  v_);
		Vec4  operator  = (const D3DXVECTOR4&  v_);
		//ベクトル型変換
		class  Vec3  Vec3(  );
	};
	//クォータニオン
	//クオータニオンは(0,0,0,0)は有効な値ではないので注意
	class QT : public  D3DXQUATERNION
	{
	public:
		QT( );
		QT::QT(float  rY_);
		QT(const  ML::Vec3&  v_,  float  r_);
		void RotAx(const  ML::Vec3&  v_,  float  r_);
		void RotX(float  rX_);
		void RotY(float  rY_);
		void RotZ(float  rZ_);
		void RotXYZ(const  ML::Vec3  rXYZ_);
		void RotYPR(const ML::Vec3 ypr);
		QT operator * ( const D3DXQUATERNION& ) const;
		QT& operator *= ( const D3DXQUATERNION& );
	};
	//行列
	class Mat4x4 : public D3DXMATRIX
	{
	public:
		Mat4x4( );
		Mat4x4(const D3DXMATRIX&  v_);
		Mat4x4  operator  = (const D3DXMATRIX&  v_);
		//単位行列
		void  Identity( );
		//ビュー行列
		void  LookAtLH(const  Vec3& pos_,  const  Vec3&  target_,  const  Vec3&  up_ );
		//射影行列
		void  PerspectiveFovLH(float  fov_,  float  aspect_,  float  nearPlane_,  float  forePlane_ );
		//固定軸回転行列
		void  RotationX(float  r_);
		void  RotationY(float  r_);
		void  RotationZ(float  r_);
		//ヨウピッチロール回転
		void  RotationYPR(float x, float y, float z);

		//任意軸回転行列
		void  RotationAxis(const  Vec3&  v_, float  r_);
		//クオータニオンで回転行列
		void  RotationQuaternion(const  QT&  r_);
		//平行移動行列
		void  Translation(const  Vec3&  p_);
		//スケーリング
		void  Scaling(const  Vec3&  s_);
		void  Scaling(float  s_);

		//逆行列生成
		Mat4x4 Inverse( );
		//転置行列生成
		Mat4x4 Transpose( ) const;
		//座標変換
		Vec3  TransformCoord(const Vec3&  p_);
		Vec3  TransformNormal(const Vec3&  v_);
	};

	//値を所定値に近づける
	void  ValueControl_ToValue(float&  v_,float  to_,  float  aja_);
	//ベクトルの水平成分の長さを規定値に近づける
	void  VectorControl_XZ_ToValue(Vec3&  v_,float  to_,  float  aja_);
	//ベクトルの長さが規定値を超えている場合、規定値まで短くする
	void  VectorControl_XZ_Cut(Vec3&  v_,float  to_);

	//色
	class Color
	{
	public:
		float  c[4];
		void Set(float  a_,float  r_,float  g_,float  b_){	c[3]=a_; c[2]=b_; c[1]=g_; c[0]=r_;}
		Color(float  a_,float  r_,float  g_,float  b_){		Set(a_, r_, g_, b_);}
		Color(float  c_[4]){								Set(c_[3], c_[2], c_[1], c_[0]);}
		Color( ){											Set(1, 0, 0, 0);}

		const float*  Ptr( ) const{return c;}
		D3DXCOLOR  D3D( ) const{	return  D3DXCOLOR(c[0], c[1], c[2], c[3]);} 
		bool  operator==(const  Color&  obj){
			return	   (c[0] == obj.c[0] &&
						c[1] == obj.c[1] &&
						c[2] == obj.c[2] &&
						c[3] == obj.c[3] );
		}
		bool  operator!=(const  Color&  obj){
			return	  !((*this) == obj );
		}
	};

	//┌───────────────────────────────────────────┐
	//｜ファイルパスから、ファイル名を削除してパスだけを取り出す							　｜
	//｜注意！パスにファイル名を含んでいない場合の動作は保障しない							　｜
	//｜履歴		：2010/01/15	須賀康之	作成										　｜
	//└───────────────────────────────────────────┘
	static string GetPath(const string& fp_)
	{
		string s = fp_;
		string::size_type id;
		id = s.find_last_of("/");
		if(string::npos == id){	id = s.find_last_of("\\");}
		if(string::npos != id){	s = s.substr(0, id);}
		return s;
	}
	//baseと同じパスのfnを作成
	static string CreateFilePath(const string&  base_, const string&  fn_)
	{
		string s = GetPath(base_);
		s += "/";
		s += fn_;
		return s;
	}

	//	メソッドにReleaseを持つオブジェクトの解放を行う
	//	Release()メソッド実行時に内部でdeleteしている物のみ有効
	template<class T>
	static void SafeRelease(T** obj_)
	{
		if(*obj_ != nullptr)
		{
			(*obj_)->Release();
			(*obj_) = nullptr;
		}
	}
	template<class T>
	static void SafeDelete(T** obj_)
	{
		if(*obj_ != nullptr)
		{
			delete (*obj_);
			(*obj_) = nullptr;
		}
	}
	template<class T>
	static void SafeDeleteArray(T** obj_)
	{
		if(*obj_ != nullptr)
		{
			delete[] (*obj_);
			(*obj_) = nullptr;
		}
	}
	static RECT Rect(int x, int y, int w, int h)
	{
		RECT t = {x, y, x + w, y + h};
		return t;
	}

	class Box2D
	{
	public:
		int x, y, w, h;
		Box2D( )
		:x(0), y(0), w(0), h(0)
		{
		}
		Box2D(int x_, int y_, int w_, int h_)
		:x(x_), y(y_), w(w_), h(h_)
		{
		}
		Box2D(const Box2D& b_)
		:x(b_.x), y(b_.y), w(b_.w), h(b_.h)
		{
		}
		bool Hit(const  Box2D& b_) const
		{
			if( x		< b_.x + b_.w	&&
				b_.x	< x + w			&&
				y		< b_.y + b_.h	&&
				b_.y	< y + h			){
				return true;
			}
			return false;
		}
		bool Hit(const  Vec2& b_) const
		{
			if( x		< b_.x	&&
				b_.x	< x + w	&&
				y		< b_.y	&&
				b_.y	< y + h	){
				return true;
			}
			return false;
		}
		bool Hit(const  POINT& b_) const
		{
			if( x		< b_.x	&&
				b_.x	< x + w	&&
				y		< b_.y	&&
				b_.y	< y + h	){
				return true;
			}
			return false;
		}
		void Offset(const  Vec2&  v_)
		{
			x += (int)v_.x;
			y += (int)v_.y;
		}
		void Offset(float x_, float y_)
		{
			x += (int)x_;
			y += (int)y_;
		}
		void Offset(int x_, int y_)
		{
			x += x_;
			y += y_;
		}
		void Offset(POINT  p_)
		{
			x += p_.x;
			y += p_.y;
		}
		RECT Rect( ) const
		{
			RECT r = {x,y,x+w,y+h};
			return r;
		}
		Box2D OffsetCopy(int x_, int y_) const
		{
			Box2D r(*this);
			r.Offset(x_, y_);
			return r;
		}
		Box2D OffsetCopy(POINT p_) const
		{
			Box2D r(*this);
			r.Offset(p_);
			return r;
		}
		Box2D OffsetCopy(float x_, float y_) const
		{
			Box2D r(*this);
			r.Offset(x_, y_);
			return r;
		}
		Box2D OffsetCopy(const  Vec2&  v_) const
		{
			Box2D r(*this);
			r.Offset(v_);
			return r;
		}
	};


	class Box3D
	{
	public:
		float x, y, z, w, h, d;
		Box3D( )
		:x(0.0f), y(0.0f), z(0.0f), w(0.0f), h(0.0f), d(0.0f)
		{
		}
		Box3D(int x_, int y_, int z_, int w_, int h_, int d_)
		:x(float(x_)), y(float(y_)), z(float(z_)), w(float(w_)), h(float(h_)), d(float(d_))
		{
		}
		Box3D(float x_, float y_, float z_, float w_, float h_, float d_)
			:x(x_), y(y_), z(z_), w(w_), h(h_), d(d_)
		{
		}
		Box3D(const Box3D&  b_)
		:x(b_.x), y(b_.y), z(b_.z), w(b_.w), h(b_.h), d(b_.d)
		{
		}
		bool Hit(const  Box3D&  b_) const
		{
			if( x		< b_.x + b_.w	&&
				b_.x	< x + w			&&
				y		< b_.y + b_.h	&&
				b_.y	< y + h			&&
				z		< b_.z + b_.d	&&
				b_.z	< z + d			){
				return true;
			}
			return false;
		}
		bool Hit(const  Vec3&  b_) const
		{
			if( x		< b_.x	&&
				b_.x	< x + w &&
				y		< b_.y	&&
				b_.y	< y + h	&&
				z		< b_.z	&&
				b_.z	< z + d	){
				return true;
			}
			return false;
		}
		void Offset(int x_, int y_, int z_){	x += x_;	y += y_;	z += z_;	}
		void Offset(float x_, float y_, float z_){	x += x_;	y += y_;	z += z_;	}
		void Offset(const  Vec3&  v_){	x += v_.x;	y += v_.y;	z += v_.z;	}
		Box3D OffsetCopy(int x_, int y_, int z_) const
		{
			Box3D r(*this);
			r.Offset(x_, y_, z_);
			return r;
		}
		Box3D OffsetCopy(float x_, float y_, float z_) const
		{
			Box3D r(*this);
			r.Offset(x_, y_, z_);
			return r;
		}
		Box3D OffsetCopy(const  Vec3&  v_) const
		{
			Box3D r(*this);
			r.Offset(v_);
			return r;
		}

		void Rotation_Box(ML::Mat4x4 matR)
		{
			ML::Vec3 rotate_Position = ML::Vec3(this->x, this->y, this->z);
			ML::Vec3 rotate_Range = ML::Vec3(this->w, this->h, this->d);
			rotate_Position = matR.TransformCoord(rotate_Position);
			rotate_Range = matR.TransformCoord(rotate_Range);

			this->x = rotate_Position.x;
			this->y = rotate_Position.y;
			this->z = rotate_Position.z;
			this->w = rotate_Range.x;
			this->h = rotate_Range.y;
			this->d = rotate_Range.z;
		}
	};
	int	CheckStrings(TCHAR* s_[], int n_, const string& c_);
	extern map<string, bool>  msgboxlog;
	void  MsgBox(const string&  ms);

}

#include "xnacollision.h"

namespace DG_{
	class  VertexBuffer;
	class  IndexBuffer;
}
namespace ML
{
	namespace Collsion	//COLLISION
	{
		//レイとポリゴンの接触判定
		bool HitRayTriangle(
			const  ML::Vec3&  p_,		//レイ始点
			const  ML::Vec3&  d_,		//レイ方向ベクトル
			const  ML::Vec3&  v0_,		//ポリゴン頂点
			const  ML::Vec3&  v1_,		//ポリゴン頂点
			const  ML::Vec3&  v2_,		//ポリゴン頂点
			float&  dist_);			//接触点までの距離（戻り値falseなら使用不可）



		//レイとバーテックスバッファの接触判定（バーテックスバッファの構造に注意）
		//頂点が３個毎に１ポリゴンを構成しているデータでなければ機能しない
		bool HitRayTriangle(
			const  ML::Vec3&  p_,						//レイ始点
			const  ML::Vec3&  d_,						//レイ方向ベクトル
			bool  qmode_,								//クイックモード（接触するポリゴンを見つけたら即終了）
			shared_ptr<DG_::VertexBuffer>		vb_,	//バーテックスバッファ
			const  UINT pos_,							//頂点情報上の頂点座標の位置（バイト数）
			float&  dist_);							//接触点までの距離（戻り値falseなら使用不可）
		//レイとバーテックスバッファの接触判定
		//頂点インデックスを含む場合
		bool HitRayTriangle(
			const  ML::Vec3&  p_,						//レイ始点
			const  ML::Vec3&  d_,						//レイ方向ベクトル
			bool  qmode_,								//クイックモード（接触するポリゴンを見つけたら即終了）
			shared_ptr<DG_::VertexBuffer>		vb_,	//バーテックスバッファ
			const  UINT pos_,							//頂点情報上の頂点座標の位置（バイト数）
			shared_ptr<DG_::IndexBuffer>			ib_,	//インデックスバッファ
			float&  dist_);							//接触点までの距離（戻り値falseなら使用不可）
		//図形クラスの基底クラス----------------------------------------------------
		class Shape
		{
		public:
			//拡張情報(接触判定で接触の有無以外の情報が必要なとき使う）
			struct  ExtParam{
				ML::Vec3	pushOut;	//排出方向ベクトル	Ray以外
				ExtParam()
					:pushOut(0, 0, 0)
				{}
			};
		protected:
			enum  KIND{ eUnNon, eAABB, eSphere, eOBB };
			KIND  kind;
		public:
			ML::Vec3			pos;	//中心点				ALL?
			ML::Vec3			size_h;	//軸ごとのサイズ(半分） AABB  OBB
			float				rad;	//半径					Sphere
			ML::QT				angle;	//向き（クオータニオン）OBB
			typedef  shared_ptr<Shape>    SP;
		protected:
			//メンバ変数のクリア（メンバを追加したら忘れずにクリア処理も追加する）
			Shape()
				:kind(Shape::eUnNon)
				, pos(0, 0, 0)
				, size_h(0, 0, 0)
				, rad(0)
				, angle(0)
			{};
			//各図形との接触判定（順次追加）
			virtual bool Hit(const class AABB&   b_, ExtParam&  ex_) = 0;		//AABBとの接触
			virtual bool Hit(const class Sphere&   b_, ExtParam&  ex_) = 0;		//球体との接触
			virtual bool Hit(const class OBB&   b_, ExtParam&  ex_) = 0;		//OBBとの接触
		public:
			virtual bool Hit(const Shape::SP  b_, ExtParam&  ex_ = ExtParam());			//接触判定
			virtual bool HitRay(const ML::Vec3&  p_,
								const ML::Vec3&  d_,
								float&  dist_) = 0;			//Rayとの接触
			virtual Shape::SP   Clone() const = 0 ;					//コピーを生成して返す
			virtual void  Offset(const  ML::Vec3  ofs_){ pos += ofs_; }	//移動させる
			virtual void  Rotation(const  ML::QT&  rot_){ angle *= rot_; }	//回転させる
		};


		//ＡＡＢＢクラス----------------------------------------------------
		class AABB : public Shape
		{
		public:
			typedef  shared_ptr<AABB>    SP;
		private:
			AABB(){}
			bool Initialize(const ML::Vec3&  p_, const ML::Vec3& s_);
			//★図形を増やしたら追加★
			bool Hit(const class AABB&   b_, ExtParam&  ex_);		//AABBとの接触
			bool Hit(const class Sphere&   b_, ExtParam&  ex_);		//球体との接触
			bool Hit(const class OBB&   b_, ExtParam&  ex_);		//OBBとの接触
		public:
			bool HitRay(const ML::Vec3&  p_,
						const ML::Vec3&  d_,
						float&  dist_);					//Rayとの接触
			static  SP  Create(const ML::Vec3&  p_, const ML::Vec3&  s_);
			Shape::SP   Clone() const;		//コピーを生成して返す
		};


		//球体クラス----------------------------------------------------
		class Sphere : public Shape
		{
		public:
			typedef  shared_ptr<Sphere>    SP;
		private:
			Sphere(){}
			bool Initialize(const ML::Vec3&  p_, float  r_);
			//★図形を増やしたら追加★
			bool Hit(const class AABB&   b_, ExtParam&  ex_);		//AABBとの接触
			bool Hit(const class Sphere&   b_, ExtParam&  ex_);		//球体との接触
			bool Hit(const class OBB&   b_, ExtParam&  ex_);		//OBBとの接触
		public:
			bool HitRay(const ML::Vec3&  p_,
						const ML::Vec3&  d_,
						float&  dist_);					//Rayとの接触
			static  Sphere*  Create(const ML::Vec3&  p_, float  r_);
			Shape::SP   Clone() const;		//コピーを生成して返す
		};


		//ＯＢＢクラス----------------------------------------------------
		class OBB : public Shape
		{
		public:
			typedef  shared_ptr<OBB>    SP;
		private:
			OBB(){}
			bool Initialize(const  ML::Vec3&  p_, const  ML::Vec3&  s_, const  ML::QT&  a_);
			//★図形を増やしたら追加★
			bool Hit(const class AABB&   b_, ExtParam&  ex_);		//AABBとの接触
			bool Hit(const class Sphere&   b_, ExtParam&  ex_);		//球体との接触
			bool Hit(const class OBB&   b_, ExtParam&  ex_);		//OBBとの接触
		public:
			bool HitRay(const ML::Vec3&  p_,
						const ML::Vec3&  d_,
						float&  dist_);					//Rayとの接触
			static  OBB*  Create(const ML::Vec3&  p_, const ML::Vec3&  s_, const ML::QT&  a_);
			Shape::SP   Clone() const;		//コピーを生成して返す
		};


	//複合図形クラス----------------------------------------------------
		class ShapeContainer
		{
		public:
			typedef  shared_ptr<ShapeContainer>    SP;
			vector<Shape*>  child;
			//↓直で書き換えると子供の図形は置いてきぼりになるので注意（privateが望ましい）
			ML::Vec3			pos;	//中心点
			ML::QT				angle;	//向き（クオータニオン）
		protected:
			ShapeContainer()
				:pos(0, 0, 0)
				, angle(0)
			{}
		private:
			bool Initialize();
		public:
			static  SP  Create();
			virtual  bool Hit(const ShapeContainer::SP  b_, struct  Shape::ExtParam&  ep_);	//接触判定
			virtual  bool Hit(const Shape::SP  b_, struct  Shape::ExtParam&  ep_);			//接触判定
			virtual  bool HitRay(const ML::Vec3&  p_,
				const ML::Vec3&  d_,
				float&  dist_);			//Rayとの接触
			virtual  ShapeContainer::SP   Clone();		//コピーを生成して返す

			virtual  void  Offset(const  ML::Vec3  ofs_);	//移動させる
			virtual  void  Rotation(const  ML::QT  rot_);	//回転させる
		};
		//複合図形クラス(カプセル型)----------------------------------------------------
		class  Capsule : public  ShapeContainer
		{
		public:
			typedef  shared_ptr<Capsule>    SP;
		private:
			Capsule(){ }
			bool Initialize(const ML::Vec3&  p_, float  tb_, float  r_);
		public:
			static  SP  Create(const ML::Vec3&  p_, float  tb_, float  r_);
		};
	}
}