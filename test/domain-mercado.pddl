(define (domain nossomercadinho)
	(:types lista objeto local contador)
	(:predicates (corredor ?c - local)
		     (produto ?p - objeto ?c - local)
		     (nocarrinho ?p - objeto)
                     (menorque ?q1 ?q2 - contador)
                     (contadorem ?q - contador)
                     (listadecompras ?l - lista ?p - produto)
                     (listavencida ?l - lista)
		     (adjacentes ?c1 ?c2 - local))
        (:functions (total-cost) - number)
	(:action MOVER
		:parameters (?c1 ?c2 - local)
		:precondition (and (corredor ?c1)
				(adjacentes ?c1 ?c2))
		:effect (and (increase (total-cost) 1) (corredor ?c2) (not (corredor ?c1))))

	(:action PEGAR-SEMCONTADOR
		:parameters (?p - objeto ?c - local ?l - lista)
		:precondition (and (corredor ?c) (produto ?p ?c)
                                   (listadecompras ?l ?p))
		:effect (and (not (listadecompras ?l ?p)) ))

        (:action TERMINEIDECOMPRAR
               :parameters (?l - lista)
               :precondition (and (not (listavencida ?l))
                             (not (exists (?p - objeto)
                                    (listadecompras ?l ?p))))
               :effect (listavencida ?l))

	;(:action PEGAR
		;:parameters (?p - objeto ?c - local ?contA ?contB - contador)
	;	:precondition (and (corredor ?c) (produto ?p ?c)
                  ;                 (not (nocarrinho ?p))
			;	   (contadorem ?contB) (menorque ?contA ?contB)
				;)
	;	:effect (and (nocarrinho ?p) (contadorem ?contA) (not (contadorem ?contB))))
)
