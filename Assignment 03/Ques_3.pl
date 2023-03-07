% Backtracking Example

eats(lion,goat).       
eats(lion,deer). 
eats(tiger,lamb).
eats(tiger,deer).

common(X,Y,Z):-eats(X,Z),eats(Y,Z).

% Write a Query to find out what is the common food which both lions and tigers eat?

% Output:
% ?- common(lion,tiger,Z).
% Z = deer ;