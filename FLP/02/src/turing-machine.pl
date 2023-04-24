#!/usr/bin/env swipl

/** \file   turing-machine.pl
 *  \author Karel Ondřej <xondre09@stud.fit.vutbr.cz>
 *  \brief  Turingův stroj.
 */

/** Přechoová funkce Turingova stroje */
:- dynamic rules/4.

/** Operace posunu doprava. */
transition(Conf_old, Conf_new) :-
    [Pre, Q_old, [H]]=Conf_old,
    Conf_new=[[H|Pre], Q_new, [' ']],
    rules(Q_old, H, Q_new, 'R').
transition(Conf_old, Conf_new) :-
    Conf_old=[Pre, Q_old, [H|Post]],
    Post\==[],
    [[H|Pre], Q_new, Post]=Conf_new,
    rules(Q_old, H, Q_new, 'R').

/** Operace posunu doleva. */
transition(Conf_old, Conf_new) :-
    [[M|Pre], Q_old, [' ']]=Conf_old,
    [Pre, Q_new, [M]]=Conf_new,
    rules(Q_old, ' ', Q_new, 'L').

transition(Conf_old, Conf_new) :-
    [[M|Pre], Q_old, [H|Post]]=Conf_old,
    (   Post \== []
    ;   H \==' '
    ),
    [Pre, Q_new, [M,H|Post]]=Conf_new,
    rules(Q_old, H, Q_new, 'L').

/** Přepis symbolu na pásce. */
transition(Conf_old, Conf_new) :-
    [Pre, Q_old, [H_old|Post]]=Conf_old,
    [Pre, Q_new, [H_new|Post]]=Conf_new,
    rules(Q_old, H_old, Q_new, H_new),
    H_new \== 'R',
    H_new \== 'L'.

child(Conf:Path, Conf_new:[Conf|Path], Closed) :-
    transition(Conf, Conf_new),
    not(memberchk(Conf_new, Closed)).

/** Běh výpočtu Turingova stroje. */
run(Tape, ConfS) :-
    (   Tape==[],
        Conf=[[], 'S', [' ']]
    ;   Conf=[[], 'S', Tape]
    ), !,
    run([Conf:[]], ConfS, []).
run([Conf:Path|_], Res, _) :-
    [_, 'F', _]=Conf,
    reverse([Conf|Path], Res).
run([Conf:Path|Queue], Res, Closed) :-
    (   setof(Child, child(Conf:Path, Child, Closed), Next)
    ;   Next=[]
    ),
    append(Queue, Next, Queue_new), !,
    run(Queue_new, Res, [Conf|Closed]).

print([]).
print([H|T]) :-
    %writeln(H),
    print_configuration(H),
    print(T).

print_configuration(Conf) :-
    [Pre, Q, Post]=Conf,
    reverse(Pre, PreR),
    string_codes(PreS, PreR),
    string_codes(PostS, Post),
    write(PreS),
    write(Q),
    writeln(PostS).

parse_rules([H], H) :- !.
parse_rules([Line|LineS], Tape) :-
    [Q_old, ' ', V_old, ' ', Q_new, ' ', V_new]=Line,
    char_type(Q_old, upper),
    char_type(Q_new, upper),
    (   char_type(V_old, lower)
    ;   V_old==' '
    ),
    (   char_type(V_new, lower)
    ;   memberchk(V_new, ['R','L',' '])
    ),
    assertz(rules(Q_old, V_old, Q_new, V_new)),
    parse_rules(LineS, Tape).

/** Turingův stroj. */
turing_machine :-
    retractall(rules/4),
    read_lines(Lines), !,
    parse_rules(Lines, Tape), !,
    run(Tape, T), !,
    print(T).

/** main */
main(_) :- 
    turing_machine,
    halt.
main(_) :-
    halt(1).

/*******************************************************************************
 * PŘEVZATÝ KÓD ZE CVIČENÍ
 ******************************************************************************/

read_line(L, C) :-
    get_char(C),
    (   isEOFEOL(C),
        L=[], !
    ;   read_line(LL, _),
        [C|LL]=L
    ).

isEOFEOL(C) :-
    (   C==end_of_file
    ;   char_code(C, Code),
        Code==10
    ).

read_lines(Ls) :-
    read_line(L, C),
    (   C==end_of_file,
        Ls=[]
    ;   read_lines(LLs),
        [L|LLs]=Ls
    ).