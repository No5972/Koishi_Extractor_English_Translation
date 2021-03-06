#include "stdafx.h"
#include "KoishiEx.h"

#pragma once
//像素格式
using namespace Koishi;

namespace KoishiNeoplePack{
	enum IMGversion:dword{
		V1 = 1, V2 = 2, V4 = 4, V5 = 5, V6 = 6, 
		VUDEF = 0, VUKNOWN = (dword)-1, VSOUND = (dword)-2, VQUOTE = (dword)-3
	};
	enum SNDversion:dword{
		VWAVE = 1, VVORBIS = 2, VMP3 = 3, 
		VSNDUDEF = 0, VSNDUKNOWN = (dword)-1, VIMAGE = (dword)-2, VSNDQUOTE = (dword)-3
	};
	enum IMGcomp:dword{
		COMP_NONE =	5, COMP_ZLIB = 6, COMP_ZLIB_DUAL = 7, COMP_UDEF = 0
	};
	class NPKobject;		//NPK对象
	class IMGobject;		//IMG对象
	class NPKentry {
	public:
		NPKentry();
		NPKentry(str newComment, long newLink);
	public:
		str comment;
		long link;
	};
	class NPKblock {
	public:
		NPKblock();
		~NPKblock();
	public:
		stream data;		//借用stream类，但不用其方法
		long start;			//只在导入导出时使用，其他时就不用了
		long length;
	public:
		void load(const stream &sourceStream);
		void destroy();
		dword checkMagic();
		dword checkData(long pos);
	};
	class NPKobject {
	public:
		NPKobject();
	public:
		std::vector < NPKentry > content;
		std::vector < NPKblock > block;
	public:
		bool loadFile(const str &fileName);	//读取文件
		bool saveFile(const str &fileName);	//保存文件
		bool create();		//新建一个空条目的NPK对象
		bool release();		//释放
	private:
		void addBlock(const stream &newStream);
	public:
		//在最后位置插入条目
		bool push(const str &path, const stream &newStream);
		bool pushFile(const str &path, str fileName);
		bool pushCopy(const str &path, long sourcePos);
		bool pushQuote(const str &path, long sourcePos);
		//从中间插入条目
		bool insert(long pos, const str &path, const stream &newStream);
		bool insertFile(long pos, const str &path, str fileName);
		bool insertCopy(long pos, const str &path, long sourcePos);
		bool insertQuote(long pos, const str &path, long sourcePos);
		//提取条目
		bool extract(long pos, stream &dest);
		bool extractFile(long pos, const str &fileName);
		//删除条目
		bool remove(long pos);
		//重命名条目
		bool rename(long pos, str newPathName);
		//查找条目
		bool find(const str &keyword, long &pos, long startPos);
		//替换条目的数据
		bool replace(long pos, const stream &newStream);
		bool replaceFile(long pos, const str &fileName);
		bool replaceCopy(long pos, long sourcePos);
		bool replaceQuote(long pos, long sourcePos);
		//为条目重新订阅数据（将资源追加到数据块表中，并重定向于它）
		bool subscribe(long pos, stream newStream);
		bool subscribeFile(long pos, const str &fileName);
		bool subscribeCopy(long pos, long sourcePos);
		//解引用
		bool dequote(long pos);
		//检查是否为前面条目的引用：若是，返回被引用条目的ID。若不是，返回自身。
		long checkQuote(long pos);
		//IMG接口
		bool IMGextract(long pos, IMGobject &io);
		bool IMGpush(const str &newPath, IMGobject &io);
		bool IMGinsert(long pos, const str &newPath, IMGobject &io);
		bool IMGremove(long pos);
		bool IMGreplace(long pos, IMGobject &io);
		bool IMGrename(long pos, const str &newPath);
		IMGversion IMGgetVersion(long pos);
		IMGversion IMGgetVersionEx(long pos);
		long IMGgetPaletteCount(long pos);
		//SND接口
		SNDversion SNDgetVersion(long pos);
		SNDversion SNDgetVersionEx(long pos);
	private:
		void nameMask(void *dest);				//名称掩码
		bool checkEntryRange(long pos);			//检查一个数字是否
		bool checkEntryRangeEx(long pos);
		bool checkBlockRange(long pos);
	public:
		long getCount() const;
		long getSize() const;
	public:
		bool loadEntry(const str &fileName);	//读取文件，仅读取路径名
	};
	class PICinfo;			//IMG内部帧数据
	class TEXinfo;			//IMG内部纹理集数据
	typedef std::vector<PICinfo> PIClist;
	typedef std::vector<TEXinfo> TEXlist;

	class PICinfo{
	public:
		colorFormat format;
		IMGcomp comp;
		dword dataSize;
		point basePt;
		size picSize;
		size frmSize;
		long linkTo;
		//双索引模式有效
		dword TEXusing;
		point TEXpointLT;
		point TEXpointRB;
	public:
		PICinfo();
	};
	class TEXinfo{
	public:
		colorFormat format;
		dword ID;
		dword compressedLength;
		dword dataLength;
		dword width;
		dword height;
		dword reserved;
	public:
		TEXinfo();
	};
	class IMGobject{
	public:
		IMGobject();
		IMGobject(stream &s);
		IMGobject(const str &fileName);
		~IMGobject();
		bool loadFile(str fileName);			//从文件中读取
		bool saveFile(str fileName);			//保存文件
		bool load(stream &s);
		bool make(stream &s);
		bool create(IMGversion ver);
		bool release();
		//贴图操作
		bool PICgetInfo(long pos, PICinfo &info);		//获取索引项信息
		bool PICgetInfoOffset(long pos, longex &off);		//获取索引项偏移量
		bool PICgetBound(long pos, long &left, long &right, long &top, long &bottom);
		bool PICgetTotalBound(long &left, long &right, long &top, long &bottom);
		bool PICgetData(long pos, stream &s);			//获取图片型数据
		bool PICgetDataOffset(long pos, longex &off);		//获取图片型数据偏移量
		bool PICpush(const PICinfo &info, const stream &s);				//插入图片
		bool PICinsert(long pos, const PICinfo &info, const stream &s);	//插入图片索引项
		bool PICremove(long pos);										//删除图片
		bool PICreplace(long pos, const PICinfo &info, const stream &s);//替换图片
		bool PICextract(long pos, image &mat, long paletteID = 0);		//提取图片
		bool PICpreprocess(const image &mat, stream &s, PICinfo &info, colorFormat cf = COLOR_UDEF, long paletteID = 0);//预处理图片
		bool PICextractIndexImage(long pos, image &mat);									//提取的矩阵是索引数据
		bool PICpreprocessIndexImage(const image &mat, stream &s, PICinfo &info);			//根据索引数据的矩阵对IMG进行预处理
		bool PICempty(stream &s, PICinfo &info);						//以空帧预处理图片
		bool PICsetInfoPara(long pos, long term, void *pval);		//设置索引项信息
		//纹理集操作
		bool TEXgetInfo(long pos, TEXinfo &info);		//获取纹理集索引项信息
		bool TEXgetInfoOffset(long pos, longex &off);		//获取纹理集索引项偏移量
		bool TEXgetData(long pos, stream &s);			//获取纹理集数据
		bool TEXgetDataOffset(long pos, longex &off);		//获取纹理集数据偏移量
		
		bool TEXpush(const TEXinfo &info, const stream &s);
		bool TEXinsert(long pos, const TEXinfo &info, const stream &s);
		bool TEXremove(long pos);
		bool TEXreplace(long pos, const TEXinfo &info, const stream &s);
		bool TEXextract(long pos, image &mat);
		bool TEXpreprocess(const image &mat, stream &s, TEXinfo &info, colorFormat cf = COLOR_UDEF);
		//颜色操作
		bool CLRpush(const color &clr, long paletteID = 0);
		bool CLRinsert(long pos, const color &clr, long paletteID = 0);
		bool CLRremove(long pos, long paletteID = 0);
		bool CLRreplace(long pos, const color &clr, long paletteID = 0);
		bool CLRnewPalette();
		bool CLRdeletePalette(long paletteID);
		//转换操作
		bool convertToV2(std::vector<IMGobject> &newIOList, colorFormat cf = ARGB8888);
		bool convertToV4(std::vector<IMGobject> &newIOList, colorList useColorPalette, bool useOriginPalette = true);
		bool convertToV5(std::vector<IMGobject> &newIOList, colorFormat cf, colorList useColorPalette, bool makeTexture);
		bool convertToV6(std::vector<IMGobject> &newIOList, colorTable useColorPaletteList, bool useOriginPalette = true);
		
		
		//其他操作
		long linkFind(dword pos, dword depth = 5);		//查找索引帧最终指向（depth为深度）
		longex getSize() const;
		bool empty();
		static void makeEmpty(IMGobject &newIO, int frameCount);
		static bool checkIsOld(stream &s);
	public:
		NPKentry *derived;

		IMGversion version;
		dword indexCount, indexSize;
		dword V5_TEXCount;
		dword V5_totalLength;

		palette paletteData;
		TEXlist TEXcontent;
		PIClist PICcontent;
	protected:
		stream *getData(uchar _part);
		stream dataHeader;		//文件头（V2,V4,V6为一组,V5为一组）
		stream dataPalette;		//调色板数据
		stream dataIndexTexture;//DDS信息，V5之外无效
		stream dataIndexPicture;//图像信息
		stream dataTexture;		//DDS图像数据，V5之外无效
		stream dataPicture;		//图像数据
	};
}

namespace KoishiExpand{
	using namespace KoishiNeoplePack;
	using namespace Koishi;
	//扩展功能
	namespace KoishiMarkTool{
		//图片内字体工具
		extern void CharMatLarge(char p, image &mat, color clr = color(0xff,0,0,0));
		extern void StrMatLarge(str s, image &mat, color clr = color(0xff,0,0,0));
		extern bool ImageMarking(const image &sourceImage, image &destImage, str codeString, point deltaPoint, color textColor);
		//PS画布风格
		extern bool ImagePSstyle(const image &sourceImage, image &destImage, color clrBound = color(0xFF, 0, 0, 0), color clrBG1 = color(0xFF, 0xFF, 0xFF, 0xFF), color clrBG2 = color(0xFF, 0xDD, 0xDD, 0xDD));
	}
	namespace KoishiDownloadTool{
		class SPKblock{
		public:
			unsigned long bzlib;		//第一个双字，若采用BZ压缩则为1
			unsigned long len;			//第二个双字，数据块长度，不包含数据块头的48字节
			unsigned long bzlib_r;		//第三个双字，第一双字反码
			unsigned long len_r;		//第四个双字，第二双字反码
			unsigned char hash[32];		//哈希32字节，再往后就是真实数据了

			unsigned long startPos;		//在原SPK文件的起始位置
			unsigned long mainStartPos;	//真实数据在原SPK文件的起始位置，配合len可以取出完整数据
		};
		typedef std::vector<SPKblock> SPKlist;
		class SPKobject{
		public:
			//头部
			unsigned long magic;			//应该是标识，均为0x1B111
			unsigned char name[260];		//文件名
			unsigned long reserve1;			//所有SPK文件内此双字均为0xC8
			unsigned long decompressed_len;	//压缩后文件大小
			unsigned char hash[32];			//哈希32字节
			unsigned long maxBlockSize;		//最大数据块中真实数据长度，似乎都是0xE1000（900K）
			unsigned long indexCount;		//数据块数目
			std::vector<unsigned long> listLen;	//数据块的长度
			//之后就是各种数据块了
			SPKlist list;
			Koishi::stream  data;
			bool load(Koishi::str fileName);
			void release();
			//提取单个数据块
			void extract(int pos, stream &s);
			void extractTrueStream(stream &s);
			void makeNPK(str NPKfileName);
		};
		class LSTfile{
		public:
			unsigned long nameLen;
			unsigned char name[256];
			unsigned long fileLen;
			unsigned long compLen;
			unsigned long hashLen;
			unsigned char hash[32];
			LSTfile();
		};
		class LSTfolder{
		public:
			unsigned long nameLen;
			unsigned char name[256];
			unsigned long fileCount;
			unsigned long subFolderCount;
			std::vector<LSTfile> list;
			LSTfolder();
		};
		class LSTobject{
		public:
			unsigned char hash[32];
			unsigned long magic;
			unsigned long reserve;
			unsigned long reserve2;
			std::vector<LSTfolder> list;
			void load(Koishi::stream &s);
			bool load(const str &fileName);
			bool getImagePack2(LSTfolder &lf);
			bool getSoundPack(LSTfolder &lf);
		};
		class TCTobject{
		public:
			unsigned char name[256];
			unsigned char ex[256];
			unsigned long magic;
			unsigned short minVersion;
			unsigned short GTBF;
			unsigned short compMethod;
			unsigned short lastTime;
			unsigned short lastDate;
			unsigned long CRC32;
			unsigned long comSize;
			unsigned long uncomSize;
			unsigned short nmSize;
			unsigned short exSize;
			unsigned long dtStartPos;
			Koishi::stream  data;
			bool load(str fileName);
			bool release();
			bool makeNPK(str NPKfileName);
		};
		class LSTfileGF{
		public:
			unsigned char name[256];
			unsigned char lenStr[32];
			unsigned char hash[32];
			LSTfileGF();
		};
		class LSTobjectGF{
		public:
			std::vector<LSTfileGF>  list;
			bool load(const str &fileName);
			bool load2(const str &fileName);
		};
	}
	class IMGobjectV1{
	//旧版本IMGV1
	protected:
		PIClist PICcontent;
		queueex posList;
		queueex lenList;
		stream data;
	public:
		bool load(stream &s);
		bool load(const str &fileName);
		bool make(IMGobject &io);
		bool release();
	};
	class exhibit{
	//多图展示
	public:
		exhibit();
	public:
		void clear();
		void create(int width, int height);
		bool putImage(const image &newMat, bool expanded = false);
	public:
		image canvas;		//大画布
	public:
		//摆放方式1·先按行摆放，每行摆放完毕后·另起一行，新行的坐标为上一行坐标的下限
		queue hList;				//每行的起始坐标(每行第一张图的y坐标)
		std::vector<queue> rList;	//指定行中每列的起始坐标(单行中每一张图的x坐标)
		int hMaxPerRow;				//当前放入行的图的最大高度，用于计算另起一行时的新y坐标
	};
	class authorLock{
	public:
		//返回0口令错误 返回1口令正确 返回2无锁 返回3无此文件或文件长度不够
		static int checkLock(str fileName, str password);
		static bool addLock(str fileName, str password);
	};
	class textDisplay{
	public:
		static void binary(const stream &in, stream &out);		//以二进制输出字节流，低位在前高位在后
		static void binaryFile(const stream &in, str fileName);	//输出文件
		static void binaryCompareFile(const stream &in1, const stream &in2, str fileName);	//输出文件
	};
	//////////////////////////////////////////////////////////////////////////////////////
}
