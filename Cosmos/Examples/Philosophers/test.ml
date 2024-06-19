#use "../testTeamCity.ml";;

test_cosmos "Philosopher color" "philo.grml" "--loop 1000"  "--max-run 20000 --const N=8"
  [("Throughput_takeLeft",0.0132773335) ; 
  ("Throughput_takeRight", 0.0052773335) ;
  ("Throughput_backToThink", 0.0052773335);
  ("MeanToken_Think", 0.0238291302177);
  ("MeanToken_Wait" , 7.96561444752);
  ("MeanToken_Eat" , 0.0105564222604);
  ("MeanToken_Fork" , 0.0132727079572);
];;

test_cosmos "Philosopher color" "philo.grml" "--loop 1000"  "--use-setdomain-impl --max-run 20000 --const N=8"
  [("Throughput_takeLeft",0.0132773335) ;
  ("Throughput_takeRight", 0.0052773335) ;
  ("Throughput_backToThink", 0.0052773335);
  ("MeanToken_Think", 0.0238291302177);
  ("MeanToken_Wait" , 7.96561444752);
  ("MeanToken_Eat" , 0.0105564222604);
  ("MeanToken_Fork" , 0.0132727079572);
  ];;

test_cosmos "Philosopher color unfold" "philo.grml" "" "--const N=8 --unfold philo_unfold.grml" [];;
test_cosmos "Philosopher color unfold " "philo_unfold.grml" "--loop 1000"  "--max-run 20000"
  [("Throughput_takeLeft_philo_IC_1",0.0132773335/.8.) ;
  ("Throughput_takeRight_philo_IC_1", 0.0052773335/.8.) ;
  ("Throughput_backToThink_philo_IC_1", 0.0052773335/.8.);
  ("MeanToken_Think_philo_IC_1", 0.0238291302177/.8.);
  ("MeanToken_Wait_philo_IC_1" , 7.96561444752/.8.);
  ("MeanToken_Eat_philo_IC_1" , 0.0105564222604/.8.);
  ("MeanToken_Fork_philo_IC_1" , 0.0132727079572/.8.);
  ];;