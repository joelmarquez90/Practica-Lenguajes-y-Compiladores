@
DECLARE
	INT : [A,B,C]
	REAL : [D,E,F]
ENDDECLARE
DECLARE
	STRING : [G]
ENDDECLARE

C = 30
WHILE (A < C OR A == C)
{
	B = 20
	WHILE (A < B)
	{
		A = A + 2
		OUT(A)
	}
	OUT ("HOLA")
	OUT(A)
	B = B - C
	B = A + 1
	A = A + 2
}
OUT(A)
OUT(B)
OUT(C)
IF (NOT EQUMIN(A;[B-8;C+4]))
{
	##C = 1
	IF (INLIST(A;[B+C;A*C]) AND EQUMAX(C;[0;1;0]))
	{
		G = "INLIST" + " AND EQUMAX"
		OUT(G)
	}
	ELSE
	{
		G = "NO" + " ENTRE" + " AL " + "IF INLIST"
		OUT(G)
	}
	A = 10
	OUT(A)
	F = 10. - D
	IF (F == 10.0)
	{
		OUT("F == 10.0")
	}
}
$