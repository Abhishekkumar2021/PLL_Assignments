countDown(0).

countDown(N) :-
    write(N), nl,
    N1 is N - 1,
    countDown(N1).

:- countDown(10), halt.