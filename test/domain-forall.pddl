(define (domain caminhando)
	(:types lugar dindin)
	(:predicates
		(Ligado ?l1 ?l2 - lugar)
		(estou-em ?l - lugar)
		(passei-em ?l - lugar)
		(comprar-dindin ?d - dindin)
		(tem-dindin ?d - dindin)
		(lugar-eh ?l1 ?l2 - lugar))
	(:action mover
		:parameters (?l1 ?l2 - lugar)
		:precondition (and (estou-em ?l1) (or (Ligado ?l1 ?l2) (Ligado ?l2 ?l1)))
		:effect (and (not (estou-em ?l1)) (passei-em ?l2) (estou-em ?l2)
			(not (lugar-eh ?l1 ?l1)) (lugar-eh ?l2 ?l2)))
	(:action comprardindin
		:parameters (?l - lugar ?d - dindin)
		:precondition (and (lugar-eh ?l DindinGourmet) (tem-dindin ?d))
		:effect (and (comprar-dindin ?d) (not (tem-dindin ?d))))
	(:action comprar-todos-dindin
		:parameters (?l - lugar)
		:precondition (lugar-eh ?l DindinGourmet)
		:effect (forall (?d - dindin) (and (comprar-dindin ?d) 
			(not (tem-dindin ?d)))))
	(:action terremoto
		:effect (forall (?l1 - lugar) (forall (?l2 - lugar)
			(not (Ligado ?l1 ?l2))))))
