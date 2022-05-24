#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <conio.h>

using byte = signed char;

class GAME {
public:
	static void Run();//�Q�[���N��
	static void Shutdown();//�V���b�g�_�E��
};

//�e�g���X�̃N���X
class TETRIS
{
public:
	byte x;//���݂�x���W
	byte y;//���݂�y���W
	byte type;//�e�g���~�m�̎��
	byte rota;//�X�s����
	byte next;//���ɏo�Ă���e�g���~�m�̎��

	unsigned Score;//�Q�[���̃X�R�A
	byte Level;//�Q�[�������N
	bool speed;//����
	bool hardDrop;//�n�[�h�h���b�v

	void Update(unsigned* i);//�X�V
	void setField(class FIELD* f);//�t�B�[���h�|�C���^���Z�b�g
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

//�t�B�[���h�`�b�v�̔z�u
class FIELD
{
public:
	std::vector<std::vector<byte>> Tile;//�`�b�v��z�u����ׂ̃^�C�����쐬
	FIELD(byte width = 20, byte height = 22);//�^�C���̕��A����
private:
	void putEmpty(byte x = 1, byte y = 0, byte width = 10, byte height = 21);//�󔒂�z�u
	void putScore(byte x = 13, byte y = 4, byte width = 3, byte height = 1);//SCORE��z�u
	void putLevel(byte x = 13, byte y = 8, byte width = 3, byte height = 1);//LEVEL��z�u
	void putNext(byte x = 13, byte y = 12, byte width = 2, byte height = 1);//NEXT��z�u
};