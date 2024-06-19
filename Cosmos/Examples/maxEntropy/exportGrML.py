import re

nbpoly= -1;
## Exporting Polynomes
def poly_to_c_first(p):
    supp=p.exponents();
    s='0.0';
    for triplet in supp:
        s+='+('+ str(p[triplet])+')';
        for j in range(cardclocks):
            if triplet[j]==1:
                s+="*param[%d]" %(j+1);
            if triplet[j]>1:
                s+="*pow(param[%d]," %(j+1)+ str(triplet[j]) +')';
        if triplet[cardclocks]==1:
            s+="*t";
        if triplet[cardclocks]>1:
            s+="*pow(t,"+ str(triplet[cardclocks])+')';
    return(s);

def poly_to_c_bis(p):
    supp=p.exponents();
    s='0.0';
    for triplet in supp:
        s+='+('+ str(p[triplet])+')';
        for j in range(cardclocks):
            if triplet[j]==1:
                s+="*x_%d" %(j+1);
            if triplet[j]>1:
                s+="*pow(x_%d," %(j+1)+ str(triplet[j]) +')';
        if triplet[cardclocks]==1:
            s+="*t";
        if triplet[cardclocks]>1:
            s+="*pow(t,"+ str(triplet[cardclocks])+')';
    return(s);

def matrix_to_c(tab,name):
    n = len(tab);
    m = len(tab[0]);
    s= 'const int '+name+'[%d]' %n +'[%d] ={ '%m;
    for v in tab:
        s+= '{ ';
        v.reverse();
        for v2 in v:
            s+= '%d, ' %v2;
        s+= '}, ';

    return s+'};\n';

def poly_to_c_ter(poly):
    s0=re.sub('\{\(','{{',str(poly.dict()))
    s1=re.sub('\, *?\(','}, {',s0)
    s2=re.sub('\):' , ',' ,s1)
    s3='0'
    for i in range(cardclocks):
        s3 = s3+',0';
    if poly != 0:
        return (s2+ ',{ ' +s3+',0}' +'}');
    else :
        return ('{{ ' +s3+',0}}');

def poly_to_c(p):
    return (poly_to_c_ter(p));

def poly_to_data(p,fichier_data,cardclocks, comment=''):
    #print poly_to_c_first(p);
    global nbpoly;
    nbpoly+=1;
    fichier_data.write((str(cardclocks+1)+','));
    s0=re.sub('\(','',str(p.dict()))
    s1=re.sub('\, *?\(',' , ',s0)
    s2=re.sub('\):' , ',' ,s1)
    s3=re.sub('}' , '' , s2)
    s4=re.sub('{' , '' , s3)
    fichier_data.write(s4);
    #fichier_data.write(comment);
    fichier_data.write('\n');
    return nbpoly;


def poly_to_c_data(p,fichier_data,cardclocks):
    n =poly_to_data(p,fichier_data,cardclocks);
    s= "{0";
    for i in range(cardclocks):
        s+=",x_%d " %(i+1);

    return ("customDistr.evalPoly(%i" %n) + (",%s,0.0})" %s);
################################################################################

def escapename(s):
    s2=s.replace('(','').replace(')','').replace('>','G').replace('<','L').replace('&','^');
    s3=s2.replace(',','_').replace('{','').replace('}','').replace('=','E').replace('-','M');
    return s3



arcCounter= -1;

def printGRML_OneArc(id,source,target):
    global arcCounter
    arcCounter+=1;
    s="";
    s+='  <arc id=\"%s' %id +'%d\"' %arcCounter +' arcType=\"arc\" source=\"%s\" ' %source + 'target=\"%s\" ' %target+'>\n';
    s+='    <attribute name=\"valuation\"><attribute name=\"expr\">\n';
    s+='      <attribute name=\"numValue\">1</attribute>\n';
    s+='    </attribute></attribute>\n';
    s+='  </arc>\n';
    return s;

def printGRML_OneArcN(id,source,target,value):
    global arcCounter
    arcCounter+=1;
    s="";
    s+='  <arc id=\"%s' %id +'%d\"' %arcCounter +' arcType=\"arc\" source=\"%s\" ' %source + 'target=\"%s\" ' %target+'>\n';
    s+='    <attribute name=\"valuation\"><attribute name=\"expr\">\n';
    s+='      <attribute name=\"numValue\">%i</attribute>\n' %value;
    s+='    </attribute></attribute>\n';
    s+='  </arc>\n';
    return s;

def printGRML_OnePlace(id,name,marking):
    s="";
    s+='  <node id=\"%s\" ' %id +' nodeType=\"place\">\n';
    s+='    <attribute name=\"name\">%s' %name + ' </attribute>\n';
    s+='    <attribute name=\"marking\"><attribute name=\"expr\">\n';
    s+='      <attribute name=\"numValue\">%d '%marking+' </attribute>\n';
    s+='    </attribute></attribute>\n';
    s+='  </node>\n';
    return(s);

def printGRML_OneTransition(id,name,distribution,priority,weight,extra):
    s="";
    s+='  <node id=\"'+ id + '\"  nodeType=\"transition\">\n';
    s+='    <attribute name=\"name\">'+ name +'</attribute>\n';
    s+='    <attribute name=\"distribution\">\n';
    s+=distribution
    s+='    </attribute>\n';
    s+='    <attribute name=\"priority\"><attribute name=\"expr\">\n';
    s+='    <attribute name=\"numValue\">'+ priority + '</attribute>\n';
    s+='    </attribute></attribute>\n';
    s+='    <attribute name=\"weight\"><attribute name=\"expr\">\n';
    s+='    '+ weight + '\n';
    s+='    </attribute></attribute>\n';
    s+='    '+extra+'\n';
    s+='  </node>\n';
    return(s);

def printGRML_OneInstaTransition(id,name,priority,weight,extra):
    s='      <attribute name=\"type\">\n';
    s+='      IMDT\n';
    s+='      </attribute>\n'
    return(printGRML_OneTransition(id,name,s,priority,weight,extra));

def printUniformDistribution(low,up):
    t='      <attribute name=\"type\">\n';
    t+='      UNIFORM\n';
    t+='      </attribute>\n';
    t+='      <attribute name="param">\n'
    t+='        <attribute name="expr"><attribute name=\"unParsed\">%s</attribute></attribute>\n' %low;
    t+='      </attribute>\n'
    t+='      <attribute name="param">\n'
    t+='        <attribute name="expr"><attribute name=\"unParsed\">%s</attribute></attribute>\n' %up;
    t+='      </attribute>\n'
    return(t);

def printDefineUserDistribution(file,name,var,nbparam, low, up, norm, cdf ,pdf):
     s ='    <attribute name=\"UserDefineDistributionPoly\">\n';
     s+='      <attribute name=\"polyDataFile\"> '+file + '.data </attribute>\n';
     s+='      <attribute name=\"name\">%s</attribute>\n' %name;
     s+='      <attribute name=\"var\">%s</attribute>\n' %var;
     s+='      <attribute name=\"nbParam\">%i</attribute>\n' %nbparam;
     s+="      <attribute name=\"lowerBound\">%i</attribute>\n" %low;
     s+="      <attribute name=\"upperBound\">%i</attribute>\n" %up;
     s+="      <attribute name=\"norm\">%i</attribute>\n" %norm;
     s+="      <attribute name=\"cdf\">%i</attribute>\n" %cdf;
     s+="      <attribute name=\"pdf\">%i</attribute>\n" %pdf;
     s+='    </attribute>\n'
     return(s);

def printUserDefineDistribution(id,param):
    t= '      <attribute name=\"type\">\n';
    t+='      USERDEFINE\n';
    t+='      </attribute>\n';
    t+='      <attribute name="param">\n'
    t+='        <attribute name="expr"><attribute name=\"unParsed\"> %s' %id;
    t+='        </attribute></attribute>\n';
    t+='      </attribute>\n';
    for c in param:
        t+='      <attribute name="param">\n';
        t+='        <attribute name="expr"><attribute name="name">\n';
        t+='          x_%d'%(c+1)+'\n';
        t+='        </attribute></attribute>\n';
        t+='      </attribute>\n';
    return(t);

def printGRML_Start(consts, clocks, distributions, external):
    s='<?xml version="1.0" encoding=\"UTF-8\"?>\n\n<model formalismUrl=\"http://formalisms.cosyverif.org/sptgd-net.fml\" xmlns=\"http://cosyverif.org/ns/model\">\n';
    s+='  <attribute name=\"declaration\">\n';
    s+='    <attribute name=\"constants\">\n';
    s+='      <attribute name=\"intConsts\">\n';
    for var,value in consts.items():
        s+='        <attribute name=\"intConst\">\n';
        s+='          <attribute name=\"name\">%s</attribute>\n' %var;
        s+='          <attribute name=\"expr\"><attribute name=\"numValue\">%i</attribute></attribute>\n' %(value);
        s+='        </attribute>\n';
    s+='      </attribute>\n';
    s+='    </attribute>\n';
    s+= distributions
    s+='    <attribute name="variables">\n';
    s+='      <attribute name="clocks">\n';
    for i in clocks:
        s+='        <attribute name="clock">\n';
        s+='        <attribute name="name"> x_%d </attribute>\n' %(i+1);
        s+='        </attribute>\n';
    s+='      </attribute>\n';
    s+='    </attribute>\n';
    s+='  </attribute>\n';
    s+='  <attribute name=\"externalDeclaration\">\n%s\n</attribute>\n' %external;
    return(s);
