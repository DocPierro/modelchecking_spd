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
  int oupx = 0; int oupy = 0; int ouvx = 0; int ouvy = 0;
  int odpx = 0; int odpy = 0; int odvx = 0; int odvy = 0;
  bool seenup = false; bool seendown = false;
  int bestup = 0; int bestdown = 0;
  int fvy = 0; int uvy = 0; int dvy = 0;
  int fvx = 0; int uvx = 0; int dvx = 0;
  int fpx = 0; int upx = 0; int dpx = 0;
  othVehicle veh;
  for (int i=0;i < nb;i++) {
    veh = donnees[i];
    opx = veh.px; opy = veh.py; ovx = veh.vx; ovy = veh.vy;
    if (opy == posy && opx > posx) {
      if (not seenother) { posbest = opx+1; seenother = true; }
      if (opx < posbest) { posbest = opx; fvy = ovy; fvx = ovx; fpx = opx; }
    }
    if (opy == posy + 1 && opx >= posx) {
      if (not seenup) { bestup = opx+1; seenup = true; }
      if (opx < bestup) { bestup = opx; uvy = ovy; uvx = ovx; fpx = opx; }
    }
    if (opy == posy - 1 && opx >= posx) {
      if (not seendown) { bestdown = opx+1; seendown = true; }
      if (opx < bestdown) { bestdown = opx; dvy = ovy; uvy = ovx; dpx = opx; }
    }
  }

  if (not seenother) { nvitx = vMax; }
  else {
    // Dans le cas où le véhicule de devant n'avance pas assez vite, voir pour changer de file
    if (not seenup) { nposy = posy+1; nvitx = vMax; }
    else {
      if (not seendown) { nposy = posy-1; nvitx = vMax; }
      else { // Tenter de se cadrer à la vitesse du plus rapide + aller sur sa voie si on peut
        nvitx = fvx;
        if (dvx > nvitx && dpx > posx) { nvitx = dvx; nposy=posy-1; }
        if (uvx > nvitx && upx > posx) { nvitx = uvx; nposy=posy+1; }
      }
    }
  }
  // Nouvelles valeurs
  egoVehicle valeurs = {posx, nposy, nvitx, vity, accx, accy};
  return valeurs;
}
