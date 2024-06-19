void magicReset() {
};

bool magicConditional(TR_PL_ID t) {
  return true;
}

void magicUpdate(TR_PL_ID t,REAL_TYPE time) {
}



egoVehicle controller(int nb, radarData& donnees, egoVehicle& ego) {
  // Séparation des données du véhicule contrôlé
  int posx = ego.px;
  int posy = ego.py;
  int vitx = ego.vx;
  int vity = ego.vy;
  int accx = ego.ax;
  int accy = ego.ay;
  // Données que l'on cherche à déterminer
  int nposx = posx; int nposy = posy; int nvitx = vitx;
  int nvity = vity; int naccx = accx; int naccy = accy;
  // Recherche du véhicule de devant parmi ceux vus
  int opx = 0; int opy = 0; int ovx = 0; int ovy = 0;
  bool seenother = false; int posbest = 0;
  othVehicle veh;
  for (int i=0;i < nb;i++) {
    veh = donnees[i];
    opx = veh.px; opy = veh.py; ovx = veh.vx; ovy = veh.vy;
    if (opy == posy) {
      if (not seenother) { posbest = opx+1; seenother = true; }
      if (opx < posbest) { posbest = opx; nvitx = ovx; }
    }
  }
  if (not seenother) { nvitx = Vmax; }
  // Nouvelles valeurs
  egoVehicle valeurs = {posx, posy, nvitx, vity, accx, accy};
  return valeurs;
}
