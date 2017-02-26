#define NUM_RANKS 8
#define NUM_FILES 8

enum Piece{
	Empty = 0,
	Pawn,//1
	Knight,
	Bishop,//3
	Rook,
	Queen,//5
	King,
	BPawn,//7
	BKnight,
	BBishop,//9
	BRook,
	BQueen,//11
	BKing,
}

typedef struct{
	Piece pieces[NUM_RANKS][NUM_FILES];
	//Other game information. Games played, etc.
} boardState;


boardState boardWipe(void){
	boardState freshBoard;
	for(int i = 0; i < NUM_RANKS; i++){
		for(int j = 0; j < NUM_FILES; j++){//Board is being filled from Bottom left(white), along the file.
			if(i = 0){ //Rank 1
				switch(j){
					case 0: 
					case 7:
						freshBoard.pieces[i][j] = Rook;
						break;
					case 1:
					case 6:
						freshBoard.pieces[i][j] = Knight;
						break;
					case 2:
					case 5:
						freshBoard.pieces[i][j] = Bishop;
						break;
					case 3:
						freshBoard.pieces[i][j] = Queen;
						break;
					case 4:
						freshBoard.pieces[i][j] = King;
						break;
				}
			}else if(i = 1){//Rank 1 -- Pawns
				freshBoard.pieces[i][j] = Pawn;
			}else if(i = 6){//Rank 6 -- BPawns
				freshBoard.pieces[i][j] = BPawn;
			}else if(i = 7){ //Rank 7
				switch(j){
					case 0: 
					case 7:
						freshBoard.pieces[i][j] = BRook;
						break;
					case 1:
					case 6:
						freshBoard.pieces[i][j] = BKnight;
						break;
					case 2:
					case 5:
						freshBoard.pieces[i][j] = BBishop;
						break;
					case 3:
						freshBoard.pieces[i][j] = BQueen;
						break;
					case 4:
						freshBoard.pieces[i][j] = BKing;
						break;
				}
			}else{
				freshBoard.pieces[i][j] = Empty;
			}
		}
	}
}


string readBoard(boardState toRead){
	string fen = "";
	int space = 0;

	for(int i = NUM_RANKS - 1; i >= 0; i--){
		for(int j = 0; j < NUM_FILES; j++){//Board is being read from top left, along each file
			if(toRead.pieces[i][j] == Empty){
				if(space > 0){
					space++//Increment
					fen[fen.length - 1] = space;//If this is not the first empty, replace the last character(space) with new empty value
				}else{
					fen = fen + "1"; //If this is the first empty in the row, place a 1
					space++;
				}
			}else{
				switch(toRead.pieces[i][j]){
					case Pawn: 
						fen = fen + "P";
						break;
					case Knight:
						fen = fen + "N";
						break;
					case Bishop:
						fen = fen + "B";
						break;
					case Rook:
						fen = fen + "R";
						break;
					case Queen:
						fen = fen + "Q";
						break;
					case King:
						fen = fen + "K";
						break;
					case BPawn:
						fen = fen + "p";
						break;
					case BKnight:
						fen = fen + "n";
						break;
					case BBishop:
						fen = fen + "b";
						break;
					case BRook:
						fen = fen + "r";
						break;
					case BQueen:
						fen = fen + "q";
						break;
					case BKing:
						fen = fen + "k";
						break;
				}
			}
		}
		space = 0;
		if(i != 0){
			fen = fen + "/";
		}
	}
}