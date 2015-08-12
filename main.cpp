/* CS 610 - Spring 2015 - Project - Ivana Seric */
/* Sparse matrix manipulation via singly linked lists */

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "SLSparseMatrix.hpp"




SLSparseMatrix * InitializeByInput( int inNRowsCols )
{
	SLSparseMatrix * myMatrix = new SLSparseMatrix(inNRowsCols);
	int 	tempRow = 0, tempCol = 0;
	double 	tempValue = 0.0;
	bool test = false;

	for (int i = 1; i <= inNRowsCols*inNRowsCols; i++)
	{
		while (!test){
			std::cout <<"\n"<< i <<". Enter non-zero elements in form:  \"row column value \", or: \"0\" for row or column index if done \n"; 
			std::cin >> tempRow >> tempCol >> tempValue; 
			if (tempRow == 0 || tempCol == 0)
				break;
			else
				test = myMatrix->CreateNode(tempRow, tempCol, tempValue);
		}
		test = false;
		if (tempRow == 0 || tempCol == 0)
			break;
	}
	return myMatrix;
}

SLSparseMatrix * InitializeByInput_A( int inNRowsCols )
{
	SLSparseMatrix * myMatrix = new SLSparseMatrix(inNRowsCols);
	int 	tempRow = 0, tempCol = 0;
	double 	tempValue = 0.0;
	bool test = false;
	int Arow[6] = {1, 2, 1, 2, 3, 4};
	int Acol[6] = {1, 2, 3, 3, 1, 4};
	double Avalue[6] = {8, 7, 6, 5, 3, 9};
	// int A[4][4] = {
	// 	{ 8, 0, 6, 0, }, 
	// 	{ 0, 7, 5, 0, }, 
	// 	{ 3, 0, 0, 0, },
	// 	{ 0, 0, 0, 9 } 
	// };

	/* watch out for different size of A  .... */
	int loopLim = (inNRowsCols*inNRowsCols > 6 ? 6 : inNRowsCols*inNRowsCols );

	for (int i = 1; i <= loopLim; i++){
		while (!test){
			tempRow = Arow[i-1];
			tempCol = Acol[i-1];
			tempValue = Avalue[i-1];
			/* watch out for different size of A  .... */
			if (tempRow > inNRowsCols || tempCol > inNRowsCols )
				test = true; //
			else
				test = myMatrix->CreateNode(tempRow, tempCol, tempValue);
		}
		test = false;
	}

	return myMatrix;
}

double Test_B( int row, int column, int inNRowsCols )
{
	return (row == column ? row : 0. );
}

double Test_C( int row, int column, int inNRowsCols )
{
	return (row == (column + 1)%inNRowsCols ? -2*column - 1 : 0. );
}

double Test_D( int row, int column, int inNRowsCols )
{
	return ( (row%2 == 0)  && (column%2 == 0) ? row + column : ( column == 3 ?  -row : 0. ) );
}

SLSparseMatrix * InitializeByFormula( double (*f)( int row, int column, int inNRowsCols ), int inNRowsCols )
{
	SLSparseMatrix * myMatrix = new SLSparseMatrix(inNRowsCols);
	int 	tempRow = 0, tempCol = 0;
	double 	tempValue = 0.0;
	bool test = false;
	for (int i = 0; i < inNRowsCols; i++)
		for (int j = 0; j < inNRowsCols; j++)
			test = myMatrix->CreateNode(i+1, j+1, (*f)(i+1, j+1, inNRowsCols ));

	return myMatrix;
}


#define MINCOL(X, Y) 	( ((X->GetColIndex()) < (Y->GetColIndex()) ) ? (X) : (Y) ) 

SLSparseMatrix * Add( SLSparseMatrix * A, SLSparseMatrix * B, int nRowsCols)
{
	SLSparseMatrix * C = new SLSparseMatrix(nRowsCols);
	Nodes * tempA = NULL; 
	Nodes * firstInRowA = NULL;
	Nodes * tempB = NULL; 
	Nodes * firstInRowB = NULL;
	double 	valueC = 0.0, valueA = 0.0, valueB = 0.0;
	int 	thisColA = 0, thisColB = 0; 
	bool	test;
	for (int i = 0; i < nRowsCols; i++){
		tempA 		= A->GetRowHead(i);
		tempB 		= B->GetRowHead(i); 
		firstInRowA	= tempA;
		firstInRowB = tempB;
		for (int j = 0; j < nRowsCols; j++){

			thisColA = tempA->GetColIndex();
			if ( thisColA == j + 1 ){
				valueA = tempA->GetValue();
				tempA = tempA->GetNextRowNode();
			}
			else if ( thisColA > j+1 ){
				valueA = 0.0;
				thisColA--;
			}
			else if ((tempA == firstInRowA && j != 0 ) || A->IsRowEmpty(i) ){
				valueA = 0.0;
			}

			thisColB = tempB->GetColIndex();
			if ( thisColB == j + 1 ){
				valueB = tempB->GetValue();
				tempB = tempB->GetNextRowNode();
			}
			else if ( thisColB > j+1 ){
				valueB = 0.0;
				thisColB--;
			}
			else if ((tempB == firstInRowB && j != 0 ) || B->IsRowEmpty(i) ){
				valueB = 0.0;
			}

			if ( (valueC = valueA + valueB)!= 0){
				test = C->CreateNode(i+1, j+1, valueC );
			}
			valueC = 0.0, valueA = 0.0, valueB = 0.0;
		}
	}
	return C;
}

SLSparseMatrix * Subtract( SLSparseMatrix * A, SLSparseMatrix * B, int nRowsCols)
{
	SLSparseMatrix * C = new SLSparseMatrix(nRowsCols);
	Nodes * tempA = NULL; 			
	Nodes * firstInRowA = NULL; 	
	Nodes * tempB = NULL; 			
	Nodes * firstInRowB = NULL;
	double 	valueC = 0.0, valueA = 0.0, valueB = 0.0;
	int 	thisColA = 0, thisColB = 0; 
	bool	test;
	for (int i = 0; i < nRowsCols; i++){
		tempA 		= A->GetRowHead(i);
		tempB 		= B->GetRowHead(i); 
		firstInRowA	= tempA;
		firstInRowB = tempB;
		for (int j = 0; j < nRowsCols; j++){

			thisColA = tempA->GetColIndex();
			if ( thisColA == j + 1 ){
				valueA = tempA->GetValue();
				tempA = tempA->GetNextRowNode();
			}
			else if ( thisColA > j+1 ){
				valueA = 0.0;
				thisColA--;
			}
			else if ((tempA == firstInRowA && j != 0 ) || A->IsRowEmpty(i) ){
				valueA = 0.0;
			}

			thisColB = tempB->GetColIndex();
			if ( thisColB == j + 1 ){
				valueB = tempB->GetValue();
				tempB = tempB->GetNextRowNode();
			}
			else if ( thisColB > j+1 ){
				valueB = 0.0;
				thisColB--;
			}
			else if ((tempB == firstInRowB && j != 0 ) || B->IsRowEmpty(i) ){
				valueB = 0.0;
			}

			if ( (valueC = valueA - valueB)!= 0){
				test = C->CreateNode(i+1, j+1, valueC );
			}
			valueC = 0.0, valueA = 0.0, valueB = 0.0;
		}
	}
	return C;
}


SLSparseMatrix *  ScalarMultiply(double scalar, SLSparseMatrix * A, int nRowsCols) 
{
	SLSparseMatrix * C = new SLSparseMatrix(nRowsCols);
	if (scalar == 0)
		return C;

	Nodes * tempA = NULL; 	
	Nodes * firstInRowA = NULL; 	
	double 	valueA = 0.0;
	int 	thisColA = 0; 
	bool	test;
	for (int i = 0; i < nRowsCols; i++){
		tempA 		= A->GetRowHead(i);
		firstInRowA	= tempA;

		for (int j = 0; j < nRowsCols; j++){
			thisColA = tempA->GetColIndex();
			if ( thisColA == j + 1 ){
				valueA = tempA->GetValue();
				tempA = tempA->GetNextRowNode();
			}
			else if ( thisColA > j+1 ){
				valueA = 0.0;
				thisColA--;
			}
			else if ((tempA == firstInRowA && j != 0 ) || A->IsRowEmpty(i) ){
				valueA = 0.0;
			}

			if ( valueA!= 0){
				test = C->CreateNode(i+1, j+1, scalar*valueA );
			}
			valueA = 0.0;
		}
	}
	return C;
}

SLSparseMatrix * MartixMultiply( SLSparseMatrix * A, SLSparseMatrix * B, int nRowsCols)
{
	SLSparseMatrix * C = new SLSparseMatrix(nRowsCols);
	Nodes * tempA = NULL; 			
	Nodes * firstInRowA = NULL; 	
	Nodes * tempB = NULL; 			
	Nodes * firstInColB = NULL;
	double 	valueA = 0.0, valueB = 0.0, tempSum = 0.0;
	int 	thisColA = 0, thisRowB = 0; 
	bool	test;


	for (int k = 0; k < nRowsCols; k++){

		tempA 		= A->GetRowHead(k);
		firstInRowA	= tempA;
	    tempSum		= 0.0;
		for (int i = 0; i < nRowsCols; i++){
			tempB 		= B->GetColHead(i); 
			tempA		= firstInRowA;
			firstInColB = tempB;
			for (int j = 0; j < nRowsCols; j++){

				thisColA = tempA->GetColIndex();
				if ( thisColA == j + 1 ){
					valueA = tempA->GetValue();
					tempA = tempA->GetNextRowNode();
				}
				else if ( thisColA > j+1 ){
					valueA = 0.0;
					thisColA--;
				}
				else if ((tempA == firstInRowA && j != 0 ) || A->IsRowEmpty(i) ){
					valueA = 0.0;
				} 

				thisRowB = tempB->GetRowIndex();
				if ( thisRowB == j + 1 ){
					valueB = tempB->GetValue();
					tempB = tempB->GetNextColNode();
				}
				else if ( thisRowB > j+1 ){
					valueB = 0.0;
					thisRowB--;
				}
				else if ((tempB == firstInColB && j != 0 ) || B->IsColEmpty(i) ){
					valueB = 0.0;
				}

				if ( valueA != 0 && valueB != 0 ){
					tempSum += valueA*valueB;
				}
				valueA = 0.0, valueB = 0.0;
			}
			if (tempSum != 0){
				test = C->CreateNode(k+1, i+1, tempSum );
			}
			tempSum = 0.0;
		}
	}
	return C;
}


SLSparseMatrix * Power(SLSparseMatrix * A, int t, int nRowsCols)
{
	SLSparseMatrix * C = new SLSparseMatrix(nRowsCols);
	SLSparseMatrix * D = new SLSparseMatrix(nRowsCols);

	if (t == 1){
		return A;
	}
	else if (t == 2){
		return MartixMultiply(A, A, nRowsCols);
	}
	else if (t % 2 == 0){
		C = Power(A, t/2, nRowsCols);
		return MartixMultiply(C,C, nRowsCols);
	}
	else {
		C = Power(A, t/2, nRowsCols);
		D = MartixMultiply(C,C, nRowsCols);
		return MartixMultiply(D, A, nRowsCols);
	}


}

SLSparseMatrix * Transpose(SLSparseMatrix * A, int nRowsCols) 
{
	SLSparseMatrix * C = new SLSparseMatrix(nRowsCols);

	Nodes * tempA = NULL; 	
	Nodes * firstInRowA = NULL; 	
	double 	valueA = 0.0;
	int 	thisColA = 0; 
	bool	test;
	for (int i = 0; i < nRowsCols; i++){
		tempA 		= A->GetRowHead(i);
		firstInRowA	= tempA;

		for (int j = 0; j < nRowsCols; j++){
			thisColA = tempA->GetColIndex();
			if ( thisColA == j + 1 ){
				valueA = tempA->GetValue();
				tempA = tempA->GetNextRowNode();
			}
			else if ( thisColA > j+1 ){
				valueA = 0.0;
				thisColA--;
			}
			else if ((tempA == firstInRowA && j != 0 ) || A->IsRowEmpty(i) ){
				valueA = 0.0;
			}

			if ( valueA!= 0){
				test = C->CreateNode(j+1, i+1, valueA );
			}
			valueA = 0.0;
		}
	}
	return C;
}

int main()
{
	std::cout << "\n  CS 610 Programming Project - Ivana Seric \n \n"; 



	std::cout << "Please enter number of rows ( = number of columns): \n \n";
	int inNRowsCols = 0;
	std::cin >> inNRowsCols;

/* Automated Input - Test cases */
	std::cout << " ~~~~~~ Automated input - test cases ~~~~~~ \n"; 
	std::cout << " ~~~~~~~~~~~~~~~~~ Step 1 ~~~~~~~~~~~~~~~~~ \nA = \n"; 
	SLSparseMatrix * myMatrixA =  InitializeByInput_A(inNRowsCols);
	myMatrixA->PrintMatrix(inNRowsCols);

	std::cout << "\nB = \n"; 
	SLSparseMatrix * myMatrixB =  InitializeByFormula(&Test_B, inNRowsCols);
	myMatrixB->PrintMatrix(inNRowsCols);

	std::cout << "\nC = \n"; 
	SLSparseMatrix * myMatrixC =  InitializeByFormula(&Test_C, inNRowsCols);
	myMatrixC->PrintMatrix(inNRowsCols);

	std::cout << "\nD = \n"; 
	SLSparseMatrix * myMatrixD =  InitializeByFormula(&Test_D, inNRowsCols);
	myMatrixD->PrintMatrix(inNRowsCols);


	std::cout << " ~~~~~~~~~~~~~~~~~ Step 2 ~~~~~~~~~~~~~~~~~\n"; 
	std::cout << "\nE  = B + D = \n"; 
	SLSparseMatrix * myMatrixE =  Add(myMatrixB, myMatrixD, inNRowsCols);
	myMatrixE->PrintMatrix(inNRowsCols);	

	std::cout << "\nF  = D - C = \n"; 
	SLSparseMatrix * myMatrixF =  Subtract(myMatrixD, myMatrixC, inNRowsCols);
	myMatrixF->PrintMatrix(inNRowsCols);	

	std::cout << "\nG  = A + B = \n"; 
	SLSparseMatrix * myMatrixG =  Add(myMatrixA, myMatrixB, inNRowsCols);
	myMatrixG->PrintMatrix(inNRowsCols);	

	std::cout << "\nH  = A - B = \n"; 
	SLSparseMatrix * myMatrixH =  Subtract(myMatrixA, myMatrixB, inNRowsCols);
	myMatrixH->PrintMatrix(inNRowsCols);	

	std::cout << "\nI  = E - F = \n"; 
	SLSparseMatrix * myMatrixI =  Subtract(myMatrixE, myMatrixF, inNRowsCols);
	myMatrixI->PrintMatrix(inNRowsCols);	

	std::cout << "\nJ  = G + H = \n"; 
	SLSparseMatrix * myMatrixJ =  Add(myMatrixG, myMatrixH, inNRowsCols);
	myMatrixJ->PrintMatrix(inNRowsCols);	


	std::cout << "\n K =  5*B  = \n"; 
	SLSparseMatrix * myMatrixK =  ScalarMultiply( 5, myMatrixB, inNRowsCols);
	myMatrixK->PrintMatrix(inNRowsCols);

	std::cout << "\n L =  8*C  = \n"; 
	SLSparseMatrix * myMatrixL =  ScalarMultiply(8 , myMatrixC, inNRowsCols);
	myMatrixL->PrintMatrix(inNRowsCols);

	std::cout << "\n M =  3*G  = \n"; 
	SLSparseMatrix * myMatrixM =  ScalarMultiply( 3, myMatrixG, inNRowsCols);
	myMatrixM->PrintMatrix(inNRowsCols);

	std::cout << "\n N =  2*H  = \n"; 
	SLSparseMatrix * myMatrixN =  ScalarMultiply( 2, myMatrixH, inNRowsCols);
	myMatrixN->PrintMatrix(inNRowsCols);

	std::cout << "\n O =  2*M  = \n"; 
	SLSparseMatrix * myMatrixO =  ScalarMultiply( 2, myMatrixM, inNRowsCols);
	myMatrixO->PrintMatrix(inNRowsCols);

	std::cout << "\n P = 3*F = \n"; 
	SLSparseMatrix * myMatrixP =  ScalarMultiply( 3, myMatrixF, inNRowsCols);
	myMatrixP->PrintMatrix(inNRowsCols);


	std::cout << " ~~~~~~~~~~~~~~~~~ Step 3 ~~~~~~~~~~~~~~~~~\n"; 
	std::cout << "\n Q = A*B = \n"; 
	SLSparseMatrix * myMatrixQ =  MartixMultiply( myMatrixA, myMatrixB, inNRowsCols);
	myMatrixQ->PrintMatrix(inNRowsCols);

	std::cout << "\n R = B*D = \n"; 
	SLSparseMatrix * myMatrixR =  MartixMultiply( myMatrixB, myMatrixD, inNRowsCols);
	myMatrixR->PrintMatrix(inNRowsCols);

	std::cout << "\n S = E*G = \n"; 
	SLSparseMatrix * myMatrixS =  MartixMultiply( myMatrixE, myMatrixG, inNRowsCols);
	myMatrixS->PrintMatrix(inNRowsCols);

	std::cout << "\n T = G*E = \n"; 
	SLSparseMatrix * myMatrixT =  MartixMultiply( myMatrixG, myMatrixE, inNRowsCols);
	myMatrixT->PrintMatrix(inNRowsCols);

	std::cout << "\n U = Q*H = \n"; 
	SLSparseMatrix * myMatrixU =  MartixMultiply( myMatrixQ, myMatrixH, inNRowsCols);
	myMatrixU->PrintMatrix(inNRowsCols);

	std::cout << "\n V = S*T = \n"; 
	SLSparseMatrix * myMatrixV =  MartixMultiply( myMatrixS, myMatrixT, inNRowsCols);
	myMatrixV->PrintMatrix(inNRowsCols);

	std::cout << "\n W = R*S = \n"; 
	SLSparseMatrix * myMatrixW =  MartixMultiply( myMatrixR, myMatrixS, inNRowsCols);
	myMatrixW->PrintMatrix(inNRowsCols);



	std::cout << "\n X = D^5 = \n"; 
	SLSparseMatrix * myMatrixX =  Power( myMatrixD, 5, inNRowsCols);
	myMatrixX->PrintMatrix(inNRowsCols);

	std::cout << "\n Y = C^8 = \n"; 
	SLSparseMatrix * myMatrixY =  Power( myMatrixC, 8, inNRowsCols);
	myMatrixY->PrintMatrix(inNRowsCols);

	std::cout << "\n Z = B^10 = \n"; 
	SLSparseMatrix * myMatrixZ =  Power( myMatrixB, 10, inNRowsCols);
	myMatrixZ->PrintMatrix(inNRowsCols);

	std::cout << "\n AA = F^2 = \n"; 
	SLSparseMatrix * myMatrixAA =  Power( myMatrixF, 2, inNRowsCols);
	myMatrixAA->PrintMatrix(inNRowsCols);

	std::cout << "\n AB = G^3 = \n"; 
	SLSparseMatrix * myMatrixAB =  Power( myMatrixG, 3, inNRowsCols);
	myMatrixAB->PrintMatrix(inNRowsCols);

	std::cout << "\n AC = A^4 = \n"; 
	SLSparseMatrix * myMatrixAC =  Power( myMatrixA, 4, inNRowsCols);
	myMatrixAC->PrintMatrix(inNRowsCols);

	std::cout << "\n AD = E^3 = \n"; 
	SLSparseMatrix * myMatrixAD =  Power( myMatrixE, 3, inNRowsCols);
	myMatrixAD->PrintMatrix(inNRowsCols);




	std::cout << "\n AE = Transpose(F) = \n"; 
	SLSparseMatrix * myMatrixAE =  Transpose( myMatrixF, inNRowsCols);
	myMatrixAE->PrintMatrix(inNRowsCols);

	std::cout << "\n AF = Transpose(E) = \n"; 
	SLSparseMatrix * myMatrixAF =  Transpose( myMatrixE, inNRowsCols);
	myMatrixAF->PrintMatrix(inNRowsCols);

	std::cout << "\n AG = Transpose(V) = \n"; 
	SLSparseMatrix * myMatrixAG =  Transpose( myMatrixV, inNRowsCols);
	myMatrixAG->PrintMatrix(inNRowsCols);

	std::cout << "\n AH = Transpose(L) = \n"; 
	SLSparseMatrix * myMatrixAH =  Transpose( myMatrixL, inNRowsCols);
	myMatrixAH->PrintMatrix(inNRowsCols);




	std::cout << "\n AI = Transpose(A+B) - Transpose(A) - Transpose(B) = \n"; 
	SLSparseMatrix * temp1 = Add(myMatrixA, myMatrixB, inNRowsCols);
	SLSparseMatrix * temp2 = Transpose( temp1, inNRowsCols);
	SLSparseMatrix * temp3 = Transpose(myMatrixA, inNRowsCols);
	SLSparseMatrix * myMatrixAI = Subtract( Subtract( temp2, temp3, inNRowsCols), Transpose(myMatrixB, inNRowsCols), inNRowsCols);
	delete temp1; 
	temp1 = NULL;
	delete temp2; 
	temp2 = NULL;
	delete temp3; 
	temp3 = NULL;
	myMatrixAI->PrintMatrix(inNRowsCols);

	std::cout << "\n AJ = Transpose(A*B) - (Transpose(B)*Transpose(A)) = \n"; 
	temp1 = MartixMultiply(myMatrixA, myMatrixB, inNRowsCols);
	temp2 = Transpose( temp1, inNRowsCols);
	temp3 = MartixMultiply(Transpose(myMatrixA, inNRowsCols), Transpose(myMatrixB, inNRowsCols), inNRowsCols);
	SLSparseMatrix * myMatrixAJ = Subtract( temp2, temp3, inNRowsCols);
	delete temp1; 
	temp1 = NULL;
	delete temp2; 
	temp2 = NULL;
	delete temp3; 
	temp3 = NULL;
	myMatrixAJ->PrintMatrix(inNRowsCols);


/* User input: */ 
	std::cout << " ~~~~~~ User input ~~~~~~ \n"; 
	SLSparseMatrix * myMatrix =  InitializeByInput(inNRowsCols);
	myMatrix->PrintMatrix(inNRowsCols);

	return 0;
}


