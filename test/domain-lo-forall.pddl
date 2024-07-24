(define (domain lightsout)
	(:types cell)
	(:predicates
		(on ?c - cell)
		(adj ?c0 ?c1 - cell)
		(always-false ?c - cell))
	(:action CLICK
		:parameters (?c - cell)
		:precondition (forall (?c - cell) (not (always-false ?c)))
		:effect (and
			(when (on ?c) (not (on ?c)))
			(when (not (on ?c)) (on ?c))
			(forall (?w - cell)
				(when (or (adj ?c ?w) (adj ?w ?c))
					(and (when (on ?w) (not (on ?w)))
					(when (not (on ?w)) (on ?w))))))))
