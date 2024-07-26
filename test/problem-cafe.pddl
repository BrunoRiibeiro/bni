(define (problem teste001)
  (:domain meucafebistro)
  (:objects cafeteira1 cafeteira2 - cafeteira
            caps1 - capsula
            chococino - capsula
            caramel - capsula
            ristreto - capsula
            starobuckso - capsula
            cafematinal - capsula
            ovomaltine - capsula
            pingado - capsula
            caneca1 caneca2 caneca3 caneca4 caneca5 - caneca)

  (:init
    (= (capsulas-cafe chococino) 10)
    (= (nivel-atual cafeteira1) 300)
    (= (nivel-atual cafeteira2) 50)
    (alguma-capsula cafeteira2)
    (tem-capsula cafeteira2 chococino)
    (alguma-capsula cafeteira1)
    (tem-capsula cafeteira1 ristreto)
    (capsula-bloqueada ristreto)
    (capsula-bloqueada chococino))

  (:goal (and
    (cafe-pronto caneca1 chococino)
    (cafe-pronto caneca2 caramel)
    (cafe-pronto caneca3 ristreto)
    (cafe-pronto caneca4 ovomaltine)))
)
