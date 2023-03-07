max(A, B, A) :- A>B, !.
max(A, B, B).
:- max(3, 4, X), write(X), nl, halt.

