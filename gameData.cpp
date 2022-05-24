#include "gameData.h"
#include <thread>
std::random_device rnd;

//置換用文字列
const std::vector<std::string> STRDATA
{
	"  ","□","■",
	"SC","OR","E ",
	"LE" ,"VE" ,"L " ,
	"NE","XT",
	"GA","ME","OV","ER",
};

//回転可能回数と原点ブロックからの相対座標を示す配列x3の四次元配列
const std::vector<std::vector<std::vector<std::vector<byte>>>>	BLOCKDATA
{
	{1, {{0, 0}, {0, 0}, {0, 0}}},//原点
	{2, {{0, 1}, {0, -1}, {0, 2}}},//TETRISBLOCK
	{4, {{0, 1}, {0, -1}, {1, -1}}},//逆L
	{4, {{0, 1}, {0, -1}, {-1, -1}}},//L
	{2, {{0, 1}, {1, 0}, {1, -1}}},//かぎ型
	{2, {{0, 1}, {-1, 0}, {-1, -1}}},//逆かぎ型
	{1, {{0, -1}, {1, 0}, {1, -1}}},//四角
	{4, {{0, 1}, {1, 0}, {-1, 0}}}//T
};

namespace device {
	byte inputKey()
	{
		if (_kbhit())
		{
			switch (byte input = _getch())
			{
			case 'w':
				return 8;
			case 'a':
				return 4;
			case 's':
				return 5;
			case 'd':
				return 6;
			case ' ':
				return 1;
			default:
				return 0;
			}
		}
		return 0;
	}

	void drawConsole(TETRIS* q, FIELD* p)
	{
		system("cls");
		for (byte y = 1; y < p->Tile.at(0).size(); y++)
		{
			for (byte x = 0; x < p->Tile.size(); x++)
			{
				switch (p->Tile.at(x).at(y))
				{
				case -1:
					std::cout << std::setw(8) << std::setfill('0') << q->Score;
					x += 3;
					break;

				case -2:
					std::cout << "　 " << std::setw(2) << std::setfill('0') << q->Level << "　 ";
					x += 3;
					break;
				default:
					for (byte i = 0; i < STRDATA.size(); i++)
					{
						if (p->Tile.at(x).at(y) == i)
						{
							std::cout << STRDATA.at(i);
							break;
						}
					}
					break;
				}

			}
			std::cout << "\n";
		}
		std::cout << std::flush;
	}
}

void GAME::Run()
{
	std::vector<std::vector<byte>> a = BLOCKDATA.at(1).at(0);
	std::cout << "┏━━┓┏┓　┏┓┏┓┏━━┓　\n┗━━┛┃┃　┃┃┃┃┗━┓┃　\n┏━━┓┃┗━┓┃┃┃┃ ┏┛┗┓\n┗┓┏┛┃┏━┛┗┛┃┃ ┃┏┓┃\n┏┛┃ ┃┃　 ┏┛┃┏┛┃┃┃\n┗━┛ ┗┛　 ┗━┛┗━┛┗┛" << std::endl;
	std::cout << "wasdキーで移動、スペースキーで回転\nEnterキーで開始します" << std::endl;
	byte c = getchar();
	TETRIS Current;
	FIELD Field;
	Current.setField(&Field);
	Current.putNext();

	for (unsigned i = 0;; i++)
	{
		Current.Update(&i);
		device::drawConsole(&Current, &Field);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void GAME::Shutdown()
{
	exit(0);
}

TETRIS::TETRIS()
{
	x = 5;
	y = 1;
	type = rnd() % 7 + 1;
	rota = 1;
	next = rnd() % 7 + 1;
	Score = 0;
	Level = 49;
	speed = false;
	hardDrop = false;
	ptr_Field = nullptr;
}

FIELD::FIELD(byte WIDTH, byte HEIGHT)
{
	Tile.resize(WIDTH);
	for (byte x = 0; x < WIDTH; x++)
	{
		Tile.at(x).resize(HEIGHT);
		for (byte y = 0; y < HEIGHT; y++)
		{
			Tile.at(x).at(y) = 2;
		}
	}

	putEmpty();
	putScore();
	putLevel();
	putNext();
}

void FIELD::putEmpty(byte x, byte y, byte width, byte height)
{
	for (byte dx = 0; dx < width; dx++)
	{
		for (byte dy = 0; dy < height; dy++)
		{
			Tile.at(x + dx).at(y + dy) = 0;
		}
	}
}

void FIELD::putScore(byte x, byte y, byte width, byte height)
{
	for (byte i = 0; i < 3; i++)
	{
		Tile.at(x + i).at(y) = 3 + i;
	}

	Tile.at(x).at(y + 1) = -1;
}

void FIELD::putLevel(byte x, byte y, byte width, byte height)
{
	for (byte i = 0; i < 3; i++)
	{
		Tile.at(x + i).at(y) = 6 + i;
	}

	Tile.at(x).at(y + 1) = -2;
}

void FIELD::putNext(byte x, byte y, byte width, byte height)
{
	for (byte i = 0; i < 2; i++)
	{
		Tile.at(x + i).at(y) = 9 + i;
	}
}

void TETRIS::Update(unsigned* i)
{
	Level = 49 + (*i / 500);
	if (Level == 59) Level++;
	speed = false;
	ctrl(device::inputKey());
	if (speed) {
		downBlock();
		Score += 100;
	}
	else if (*i % abs(59 - Level) == 0)
	{
		downBlock();
	}
	else if (hardDrop) {
		for (;;) {
			Score += 100;
			downBlock();
			if (!hardDrop)
				break;
		}
	}
}

void TETRIS::ctrl(byte k)
{
	TETRIS n = *this;
	switch (k)
	{
	case 8:
		hardDrop = true;
		break;
	case 4:
		n.x--;
		break;
	case 5:
		speed = true;
		break;
	case 6:
		n.x++;
		break;
	case 1:
		n.rota++;
		break;
	default:
		break;
	}

	if (n.x != x || n.y != y || n.rota != rota)
	{
		deleteBlock();
		if (n.putBlock())
		{
			this->x = n.x;
			this->y = n.y;
			this->rota = n.rota;
		}
		else
		{
			putBlock();
		}
	}
}

void TETRIS::setField(class FIELD* f)
{
	ptr_Field = f;
}

bool TETRIS::putBlock(bool action)
{
	if (ptr_Field->Tile.at(x).at(y) != 0)
		return false;
	if (action)
		ptr_Field->Tile.at(x).at(y) = 1;
	for (int i = 0; i < 3; i++)
	{
		byte dx = BLOCKDATA.at(type).at(0).at(i).at(0);
		byte dy = BLOCKDATA.at(type).at(0).at(i).at(1);
		byte r = rota % BLOCKDATA.at(type).capacity();

		for (int j = 0; j < r; j++)
		{
			byte nx = dx, ny = dy;
			dx = ny;
			dy = -nx;
		}

		if (ptr_Field->Tile.at(x + dx).at(y + dy) != 0)
			return false;
		if (action)
			ptr_Field->Tile.at(x + dx).at(y + dy) = 1;
	}
	if (!action)
		putBlock(true);
	return true;
}

void TETRIS::deleteBlock()
{
	ptr_Field->Tile.at(x).at(y) = 0;
	for (int i = 0; i < 3; i++)
	{
		byte dx = BLOCKDATA.at(type).at(0).at(i).at(0);
		byte dy = BLOCKDATA.at(type).at(0).at(i).at(1);
		byte r = rota % BLOCKDATA.at(type).capacity();

		for (int j = 0; j < r; j++)
		{
			byte nx = dx, ny = dy;
			dx = ny;
			dy = -nx;
		}
		ptr_Field->Tile.at(x + dx).at(y + dy) = 0;
	}
}

void TETRIS::downBlock()
{
	deleteBlock();
	y++;
	if (!putBlock())
	{
		y--;
		putBlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		x = 5;
		y = 1;
		type = next;
		next = rnd() % 7 + 1;
		putNext();
		rota = 0;
		hardDrop = false;
		deleteLine();
		if (!putBlock()) {
			putBlock(true);
			GameOver();
		}
	}
}

void TETRIS::putNext()
{
	for (byte dy = 0; dy < 5; dy++)
	{
		for (byte dx = 0; dx < 5; dx++)
		{
			ptr_Field->Tile.at(13 + dx).at(14 + dy) = 0;
		}
	}

	ptr_Field->Tile.at(15).at(16) = 1;
	for (byte i = 0; i < 3; i++)
	{
		byte dx = BLOCKDATA.at(next).at(0).at(i).at(0);
		byte dy = BLOCKDATA.at(next).at(0).at(i).at(1);
		ptr_Field->Tile.at(15 + dx).at(16 + dy) = 1;
	}
}

void TETRIS::deleteLine(byte x, byte y, byte width, byte height)
{
	std::vector<byte> Line;
	byte bonus = 1;
	for (byte y = 0; y <= height; y++)
	{
		bool check = true;
		for (byte x = 1; x <= width; x++)
		{
			if (ptr_Field->Tile.at(x).at(y) == 0)
			{
				check = false;
			}
		}
		if (check)Line.push_back(y);
	}

	for (const byte& e : Line)
	{
		if (e != 0)
		{
			for (byte x = 1; x <= width; x++)
			{
				ptr_Field->Tile.at(x).at(e) = 0;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				device::drawConsole(this, ptr_Field);
			}

			for (byte y = e; y > 0; y--)
			{
				for (byte x = 1; x <= width; x++)
				{
					ptr_Field->Tile.at(x).at(y) = ptr_Field->Tile.at(x).at(y - 1);
					ptr_Field->Tile.at(x).at(y - 1) = 0;
				}
			}
			bonus++;
		}
	}
	Score += bonus * 10000;

}

void TETRIS::GameOver(byte x, byte y, byte width, byte height)
{
	for (byte dy = height; 0 <= dy; dy--)
	{
		for (byte dx = 0; dx < width; dx++)
		{
			ptr_Field->Tile.at(x + dx).at(y + dy) = 2;
		}
		device::drawConsole(this, ptr_Field);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	ptr_Field->Tile.at(3).at(10) = 11;
	ptr_Field->Tile.at(4).at(10) = 12;
	ptr_Field->Tile.at(6).at(10) = 13;
	ptr_Field->Tile.at(7).at(10) = 14;
	device::drawConsole(this, ptr_Field);
	std::cout << "ムネン\n　アトヲ\n　　タノム(´；ω；`)" << std::endl;
	byte c = getchar();
	GAME::Shutdown();
}