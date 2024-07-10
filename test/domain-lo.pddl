(define (domain lights-out)
	(:types cell)
	(:predicates
		(bulb ?c - cell)
		(broken ?c - cell)
		(adjacent-vertex ?c ?c1 ?c2 - cell)
		(adjacent-edge ?c ?c1 ?c2 ?c3 - cell)
		(adjacent ?c ?c1 ?c2 ?c3 ?c4 - cell))
	(:action CLICK-VERTEX
		:parameters (?c ?c1 ?c2 - cell)
		:precondition (and (adjacent-vertex ?c ?c1 ?c2))
		:effect (and
			(when (not (broken ?c)) (and
  				(when (bulb ?c) (not (bulb ?c)))
				(when (not (bulb ?c)) (bulb ?c))))
			(when (bulb ?c1) (not (bulb ?c1)))
			(when (not (bulb ?c1)) (bulb ?c1))
			(when (bulb ?c2) (not (bulb ?c2)))
			(when (not (bulb ?c2)) (bulb ?c2))))
	(:action CLICK-EDGE
		:parameters (?c ?c1 ?c2 ?c3 - cell)
		:precondition (and (adjacent-edge ?c ?c1 ?c2 ?c3))
		:effect (and
			(when (not (broken ?c)) (and
  				(when (bulb ?c) (not (bulb ?c)))
				(when (not (bulb ?c)) (bulb ?c))))
			(when (bulb ?c1) (not (bulb ?c1)))
			(when (not (bulb ?c1)) (bulb ?c1))
			(when (bulb ?c2) (not (bulb ?c2)))
			(when (not (bulb ?c2)) (bulb ?c2))
			(when (bulb ?c3) (not (bulb ?c3)))
			(when (not (bulb ?c3)) (bulb ?c3))))
	(:action CLICK
		:parameters (?c ?c1 ?c2 ?c3 ?c4 - cell)
		:precondition (and (adjacent ?c ?c1 ?c2 ?c3 ?c4))
		:effect (and
			(when (not (broken ?c)) (and
  				(when (bulb ?c) (not (bulb ?c)))
				(when (not (bulb ?c)) (bulb ?c))))
			(when (bulb ?c1) (not (bulb ?c1)))
			(when (not (bulb ?c1)) (bulb ?c1))
			(when (bulb ?c2) (not (bulb ?c2)))
			(when (not (bulb ?c2)) (bulb ?c2))
			(when (bulb ?c3) (not (bulb ?c3)))
			(when (not (bulb ?c3)) (bulb ?c3))
			(when (bulb ?c4) (not (bulb ?c4)))
			(when (not (bulb ?c4)) (bulb ?c4))))
)
