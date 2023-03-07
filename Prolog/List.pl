# Homogenous lists

member(X,[X|_]).
member(X,[_|T]) :- member(X,T).

append([],L,L).
append([H|T],L,[H|R]) :- append(T,L,R).

reverse([],[]).
reverse([H|T],R) :- reverse(T,R1), append(R1,[H],R).



