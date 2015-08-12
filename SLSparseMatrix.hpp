/* CS 610 - Spring 2015 - Project - Ivana Seric */
/* Sparse matrix manipulation via singly linked lists */
/* S L Sparse Matrix class */


#include <iostream>
#include <cstdlib>
#include "nodes.hpp"

class SLSparseMatrix
{
	private:
		int nRowsCols;  // matrix is  NxN
		Nodes ** rowHeads;
		Nodes ** colHeads;

	public:
		SLSparseMatrix(int inNRowsCols);
		~SLSparseMatrix();

		void 	PrintMatrix(int nRowsCols);

		void 	SetRowHead(int rowIndex, Nodes * firstNode);
		Nodes * GetRowHead(int rowIndex);
		void 	SetColHead(int colIndex, Nodes * firstNode);
		Nodes * GetColHead(int colIndex);

		Nodes * GetFirstNodeRow(int rowIndex);
		Nodes * GetFirstNodeCol(int colIndex);

		Nodes * GetLastNodeRow(int rowIndex, Nodes * oldFirst);
		Nodes * GetLastNodeCol(int colIndex, Nodes * oldFirst);

		bool	IsRowEmpty(int rowIndex);
		bool 	IsColEmpty(int colIndex);
		int 	NonZeroInRow(int rowIndex);
		int 	NonZeroInCol(int colIndex);
		Nodes * TraverseToNodeRow(int rowIndex, int colIndex);
		Nodes * TraverseToNodeCol(int rowIndex, int colIndex);
		Nodes *	NodeExists(int rowIndex, int colIndex, double value);
		void	ReplaceNode(Nodes * oldNode, double newValue);
		void	ConnectNode(Nodes * newNode);
		bool	CreateNode(int rowIndex, int colIndex, double value);
		void	DeleteNode(Nodes * oldNode);

		void 	InitializeByInput(int inNRowsCols);
}; 

SLSparseMatrix::SLSparseMatrix(int inNRowsCols)
{
	nRowsCols = inNRowsCols;
	rowHeads = new Nodes * [inNRowsCols];
	colHeads = new Nodes * [inNRowsCols];
	for (int i = 0; i < inNRowsCols; i++){  	 /* index starts from 0 */
		rowHeads[i] =  (Nodes*)&rowHeads[i]; 	 /* Not exactly pointing to itself, but what is the point anyway? ;) */
		colHeads[i] =  (Nodes*)&colHeads[i];
	}
}

SLSparseMatrix::~SLSparseMatrix()
{
}

bool SLSparseMatrix::IsRowEmpty(int rowIndex)	/* index already -1 */
{
	if (rowHeads[rowIndex] == (Nodes*)&rowHeads[rowIndex]){
		// std::cout <<"Row "<< rowIndex << " is empty. \n" ;
		return true;
	}
	return false;
}

bool SLSparseMatrix::IsColEmpty(int colIndex)  	/* index already -1 */
{
	if (colHeads[colIndex] == (Nodes*)&colHeads[colIndex]){
		// std::cout <<"Column "<< colIndex << " is empty. \n" ;
		return true;
	}
	return false;
}


void SLSparseMatrix::PrintMatrix(int nRowsCols) 
{
	Nodes * temp = NULL;// rowHeads[0];
	Nodes * firstInRow = NULL; //rowHeads[0];
	int thisCol = 1;

	for (int i = 0; i < nRowsCols; i++){
		temp = rowHeads[i];
		firstInRow = temp; 
		thisCol = 1;
		for (int j = 0; j < nRowsCols; j++)
		{
			thisCol = temp->GetColIndex();
			if ( thisCol == j + 1 ){
				printf("%10g ", temp->GetValue() );
				temp = temp->GetNextRowNode();
			}
			else if ( thisCol > j+1 ){
				printf("%10g ", 0.0 );
				// std::cout << 0. << "\t";
				thisCol--;
			}
			else if ((temp == firstInRow && j != 0 ) || IsRowEmpty(i) ){
				printf("%10g ", 0.0 );
			}
		}
		std::cout << " \n";
	}
	std::cout << "\n";
}

/* index already -1 */
void SLSparseMatrix::SetRowHead(int rowIndex, Nodes * firstNode)
{
	rowHeads[rowIndex] = firstNode;
}

Nodes * SLSparseMatrix::GetRowHead(int rowIndex)
{
	return rowHeads[rowIndex];
}

void SLSparseMatrix::SetColHead(int colIndex, Nodes * firstNode)
{
	colHeads[colIndex] = firstNode;
}

Nodes * SLSparseMatrix::GetColHead(int colIndex)
{
	return colHeads[colIndex];
}

Nodes * SLSparseMatrix::GetFirstNodeRow(int rowIndex)
{
	return rowHeads[rowIndex];
}

Nodes * SLSparseMatrix::GetFirstNodeCol(int colIndex)
{
	return colHeads[colIndex];
}

Nodes * SLSparseMatrix::GetLastNodeRow(int rowIndex, Nodes * oldFirst)
{
	Nodes * temp = oldFirst;
	if ( IsRowEmpty(rowIndex-1) )
		return temp;
	if ( oldFirst->GetNextRowNode() == oldFirst)  /*the only element in the row*/
		return oldFirst;
	while ( temp->GetNextRowNode() != oldFirst && temp->GetNextRowNode() != NULL ){
		temp = temp->GetNextRowNode();
	}
	return temp;
}

Nodes * SLSparseMatrix::GetLastNodeCol(int colIndex, Nodes * oldFirst)
{
	Nodes * temp = oldFirst;
	if ( IsColEmpty(colIndex-1) )
		return temp;
	if ( oldFirst->GetNextColNode() == oldFirst) /*the only element in the column*/
		return oldFirst;
	while ( temp->GetNextColNode() != oldFirst && temp->GetNextRowNode() != NULL ) {
		temp = temp->GetNextColNode();
	}
	return temp;
}


/* Returns a node in the same row before the position in the input  
*	rowIndex and colIndex are acctual matrix indecies 
*/
Nodes * SLSparseMatrix::TraverseToNodeRow(int rowIndex, int colIndex) 
{
	Nodes * temp = NULL;
	temp = rowHeads[rowIndex-1];		/* index starts from 0  */
	if ( IsRowEmpty(rowIndex-1) )
		return temp;
	Nodes * firstNode = rowHeads[rowIndex-1];
	while ((temp->GetNextRowNode() != firstNode) && ( (temp->GetNextRowNode())->GetColIndex() < colIndex) ){
		temp = temp->GetNextRowNode();
	}
	return temp;
}

/* Returns a node in the same column before the position in the input  
*	rowIndex and colIndex are acctual matrix indecies  
*/
Nodes * SLSparseMatrix::TraverseToNodeCol(int rowIndex, int colIndex) 
{
	Nodes * temp = NULL;
	temp = colHeads[colIndex-1];		/* index starts from 0  */
	if ( IsColEmpty(colIndex-1) )
		return temp;
	Nodes * firstNode = colHeads[colIndex-1];
	while ((temp->GetNextColNode() != firstNode) && ( (temp->GetNextColNode())->GetRowIndex() < rowIndex) ){
		temp = temp->GetNextColNode();
	}
	return temp;
}

void SLSparseMatrix::ReplaceNode(Nodes * oldNode, double newValue)
{
	oldNode->SetValue(newValue);
}

void SLSparseMatrix::DeleteNode(Nodes * oldNode)
{
	Nodes * temp = NULL;
	int oldRow = oldNode->GetRowIndex();
	int oldCol = oldNode->GetColIndex();
	Nodes * prevInRow = TraverseToNodeRow(oldRow, oldCol);
	Nodes * prevInCol = TraverseToNodeCol(oldRow, oldCol);

	if ( oldNode  == rowHeads[oldRow-1] ){
	/* oldNode is the first in the row */
		if ( oldNode->GetNextRowNode() == rowHeads[oldRow-1]  ) {// the only node
			rowHeads[oldRow-1] =  (Nodes*)&rowHeads[oldRow-1];  // head "points" to itself (holds it own address)
		}
		else {
			rowHeads[oldRow-1] = oldNode->GetNextRowNode();
			/* Last node was pointing to oldNode */
			temp = GetLastNodeRow(oldRow, oldNode);
			temp->SetNextRowNode(rowHeads[oldRow-1] );
		}
	}
	else if ( oldNode->GetNextRowNode() == rowHeads[oldRow-1] ){
	/* oldNode is the last in the row */
		std::cout<< "oldNode is the last in the row\n";
		prevInRow->SetNextRowNode(rowHeads[oldRow-1]);
	}
	else{
		prevInRow->SetNextRowNode(oldNode->GetNextRowNode() );
	}
	temp = NULL;

	if ( oldNode  == colHeads[oldCol-1] )
	{
	/* oldNode is the first in the column */
		if ( oldNode->GetNextColNode() == colHeads[oldCol-1]  ) {// the only node
			colHeads[oldCol-1] =  (Nodes*)&colHeads[oldCol-1];  // head "points" to itself (holds it own address)
		}
		else{ 
			colHeads[oldCol-1] = oldNode->GetNextColNode();
			/* Last node was pointing to oldNode: */
			temp = GetLastNodeCol(oldCol, oldNode);
			temp->SetNextColNode(colHeads[oldCol-1]);
		}
	}
	else if ( oldNode->GetNextColNode() == colHeads[oldCol-1] ){
		/* oldNode is the last in the column */
		prevInCol->SetNextColNode(colHeads[oldCol-1]);
	}
	else{
		prevInCol->SetNextColNode(oldNode->GetNextColNode());
	}
}


Nodes * SLSparseMatrix::NodeExists(int rowIndex, int colIndex, double value) 
{
	if (!IsRowEmpty(rowIndex - 1)){
		Nodes * closestNode = TraverseToNodeRow(rowIndex, colIndex);
		if ( closestNode == rowHeads[rowIndex - 1] && closestNode->GetColIndex() == colIndex) { 
			return closestNode;
		}
		else if (closestNode->GetNextRowNode() == rowHeads[rowIndex - 1]){  // closest node is the last in the row
			return NULL;
		}
		else if ((closestNode->GetNextRowNode())->GetColIndex() == colIndex ){
			return closestNode->GetNextRowNode();
		}
		else
			return NULL;
	}
	else
		return NULL;
}

void SLSparseMatrix::ConnectNode(Nodes * newNode)
{
	Nodes * temp = NULL;
	int newRow = newNode->GetRowIndex();
	int newCol = newNode->GetColIndex();
	Nodes * prevInRow = NULL;
	Nodes * prevInCol = NULL;
	if ( IsRowEmpty(newRow-1) ){
		rowHeads[newRow-1] = newNode;
		newNode->SetNextRowNode(newNode);  /* Only node: points to itself */
	}
	else if (  (prevInRow = TraverseToNodeRow(newRow, newCol))->GetColIndex()  > newCol ) /*whatever head points to is already too far*/
	{
	/* newNode is the first in the row */
		rowHeads[newRow-1] = newNode;
		newNode->SetNextRowNode(prevInRow);
		/* Last node points to first, i.e. newNode: */
		temp = GetLastNodeRow(newRow, prevInRow);
		temp->SetNextRowNode(newNode);
	}
	else {
		prevInRow = TraverseToNodeRow(newRow, newCol);
		temp = prevInRow->GetNextRowNode();
		prevInRow->SetNextRowNode(newNode);
		newNode->SetNextRowNode(temp);
	}
	temp = NULL;
	if ( IsColEmpty(newCol-1) ){
		colHeads[newCol-1] = newNode;
		newNode->SetNextColNode(newNode);  /* Only node: points to itself */
	}
	else if ( (prevInCol = TraverseToNodeCol(newRow, newCol))->GetRowIndex()  > newRow ) /*whatever head points to is already too far*/
	{
	/* newNode is the first in the column */
		colHeads[newCol-1] = newNode;
		newNode->SetNextColNode(prevInCol);
		/* Last node points to first, i.e. newNode: */
		temp = GetLastNodeCol(newCol, prevInCol);
		temp->SetNextColNode(newNode);
	}
	else {
		prevInCol = TraverseToNodeCol(newRow, newCol);
		temp = prevInCol->GetNextColNode();
		prevInCol->SetNextColNode(newNode);
		newNode->SetNextColNode(temp);
	}
}

bool SLSparseMatrix::CreateNode(int rowIndex, int colIndex, double value)
{
	if ( (rowIndex > nRowsCols) || (colIndex > nRowsCols) ){
		std::cout << " Invalid index: matrix is "<< nRowsCols << "x" << nRowsCols << ".\n"; 
		return false;
	}
	Nodes * oldNode = NodeExists(rowIndex, colIndex, value); 
	if (oldNode){
		fprintf(stderr, "%s\n", "Value already entered at that position => replacing the value." );
		if (value == 0){
			this->DeleteNode(oldNode);
			delete oldNode;
			oldNode = NULL;
			return false;
		}
		else {
			this->ReplaceNode(oldNode, value);
			return false;
		}
	}
	if (value == 0)
		return true;
	
	Nodes * newNode = new Nodes(rowIndex, colIndex, value);
	ConnectNode(newNode);
	return true;
}
