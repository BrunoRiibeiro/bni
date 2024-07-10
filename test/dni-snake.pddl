(define(domain snake)(:requirements :strips :negative-preconditions)(:constants dummypoint)(:predicates (ISADJACENT ?x ?y)(tailsnake ?x)(headsnake ?x)(nextsnake ?x ?y)(blocked ?x)(spawn ?x)(NEXTSPAWN ?x ?y)(ispoint ?x))(:action move :parameters (?head ?newhead ?tail ?newtail) :precondition (and(headsnake ?head)(ISADJACENT ?head ?newhead)(tailsnake ?tail)(nextsnake ?newtail ?tail)(not(blocked ?newhead))(not(ispoint ?newhead))) :effect (and(blocked ?newhead)(headsnake ?newhead)(nextsnake ?newhead ?head)(not(headsnake ?head))(not(blocked ?tail))(not(tailsnake ?tail))(not(nextsnake ?newtail ?tail))(tailsnake ?newtail)))(:action move-and-eat-spawn :parameters (?head ?newhead ?spawnpoint ?nextspawnpoint) :precondition (and(headsnake ?head)(ISADJACENT ?head ?newhead)(not(blocked ?newhead))(ispoint ?newhead)(spawn ?spawnpoint)(NEXTSPAWN ?spawnpoint ?nextspawnpoint)(not(= ?spawnpoint dummypoint))) :effect (and(blocked ?newhead)(headsnake ?newhead)(nextsnake ?newhead ?head)(not(headsnake ?head))(not(ispoint ?newhead))(ispoint ?spawnpoint)(not(spawn ?spawnpoint))(spawn ?nextspawnpoint)))(:action move-and-eat-no-spawn :parameters (?head ?newhead) :precondition (and(headsnake ?head)(ISADJACENT ?head ?newhead)(not(blocked ?newhead))(ispoint ?newhead)(spawn dummypoint)) :effect (and(blocked ?newhead)(headsnake ?newhead)(nextsnake ?newhead ?head)(not(headsnake ?head))(not(ispoint ?newhead)))))
