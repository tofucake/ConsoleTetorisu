#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <conio.h>

using byte = signed char;

class GAME {
public:
	static void Run();//ゲーム起動
	static void Shutdown();//シャットダウン
};

//テトリスのクラス
class TETRIS
{
public:
	byte x;//現在のx座標
	byte y;//現在のy座標
	byte type;//テトリミノの種類
	byte rota;//スピン回数
	byte next;//次に出てくるテトリミノの種類

	unsigned Score;//ゲームのスコア
	byte Level;//ゲームランク
	bool speed;//加速
	bool hardDrop;//ハードドロップ

	void Update(unsigned* i);//更新
	void setField(class FIELD* f);//フィールドポインタをセット
	void GameOver(byte x = 1, byte y = 0, byte width = 10, byte height = 20);
	void putNext();
	TETRIS();

private:
	FIELD* ptr_Field;
	void ctrl(byte k);
	bool putBlock(bool action = false);
	void deleteBlock();
	void downBlock();
	void deleteLine(byte x = 1, byte y = 0, byte width = 10, byte height = 20);
};

//フィールドチップの配置
class FIELD
{
public:
	std::vector<std::vector<byte>> Tile;//チップを配置する為のタイルを作成
	FIELD(byte width = 20, byte height = 22);//タイルの幅、高さ
private:
	void putEmpty(byte x = 1, byte y = 0, byte width = 10, byte height = 21);//空白を配置
	void putScore(byte x = 13, byte y = 4, byte width = 3, byte height = 1);//SCOREを配置
	void putLevel(byte x = 13, byte y = 8, byte width = 3, byte height = 1);//LEVELを配置
	void putNext(byte x = 13, byte y = 12, byte width = 2, byte height = 1);//NEXTを配置
};