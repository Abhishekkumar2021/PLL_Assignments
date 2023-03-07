% Cut Example: Prolog program to insert an element into a list, if it is not present in the list before. And if 
% the list has the element before we will simply cut it. For the membership checking also, if the item is at 
% the head part, we should not check further, so cut it, otherwise check into the tail part.

list_member(X,[X|_]) :- !.                      % If the item is at the head part, we should not check further, so cut it.
list_member(X,[_|T]) :- list_member(X,T).       % Otherwise check into the tail part.

list_append(A,T,T) :- list_member(A,T),!.       % If the list has the element before we will simply cut it.
list_append(A,T,[A|T]).                         % Otherwise insert the element into the list.
