/* CS 610 - Spring 2015 - Project - Ivana Seric */
/* Sparse matrix manipulation via singly linked lists */
/* Nodes class */

#include <iostream>
#include <cstdlib>


class Nodes
{
    private:
		double 	value;
		int 	rowIndex;
		int 	colIndex;
		Nodes * row_link;
		Nodes * column_link;

    public:
		Nodes(int rowIndex, int colIndex, double value);
		~Nodes();

		void 	PrintNode(); 
		
		double 	GetValue();
		void 	SetValue(double inValue);
		
		int 	GetRowIndex();
		int 	GetColIndex();
		
		void 	SetNextRowNode(Nodes * inNextRow);
		Nodes * GetNextRowNode();

		void 	SetNextColNode(Nodes * inNextCol);
		Nodes * GetNextColNode();
};

Nodes::Nodes(int inRowIndex, int inColIndex, double inValue)
{
	value = inValue;
	rowIndex = inRowIndex;
	colIndex = inColIndex;
	row_link = NULL;
	column_link = NULL;
}

Nodes::~Nodes()
{
}

void Nodes::PrintNode()
{
	std::cout << "Row " << rowIndex << ", column " << colIndex << ", value " << value << ", at address " << this <<", nextRowNode " << row_link<< ", nextColNode " << column_link<< ". \n";
}

double Nodes::GetValue()
{
	return value;
}

void Nodes::SetValue(double inValue)
{
	value = inValue;
}

int Nodes::GetRowIndex()
{
	return rowIndex;
}

int Nodes::GetColIndex()
{
	return colIndex;
}

void Nodes::SetNextRowNode(Nodes * inNextRowNode)
{
	row_link = inNextRowNode;
}

Nodes * Nodes::GetNextRowNode()
{
	return row_link;
}

void Nodes::SetNextColNode(Nodes * inNextColNode)
{
	column_link = inNextColNode;
}

Nodes * Nodes::GetNextColNode()
{
	return column_link;
}
