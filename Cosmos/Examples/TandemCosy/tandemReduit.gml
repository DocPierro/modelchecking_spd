<?xml version="1.0" encoding="UTF-8"?>

<model formalismUrl="http://alligator.lip6.fr/sptgd-net.fml" xmlns="http://cosyverif.org/ns/model">
  <attribute name="declaration">
    <attribute name="constants">
      <attribute name="intConsts">
        <attribute name="intConst">
          <attribute name="name">
            R
          </attribute>
          <attribute name="expr">
            <attribute name="numValue">
              200
            </attribute>
          </attribute>
        </attribute>
      </attribute>
      <attribute name="realConsts">
        <attribute name="realConst">
          <attribute name="name">
            rho1
          </attribute>
          <attribute name="expr">
            <attribute name="numValue">
              0.34
            </attribute>
          </attribute>
        </attribute>
        <attribute name="realConst">
          <attribute name="name">
            rho0
          </attribute>
          <attribute name="expr">
            <attribute name="numValue">
              0.32
            </attribute>
          </attribute>
        </attribute>
      </attribute>
    </attribute>
  </attribute>
  <node id="3" nodeType="place">
    <attribute name="name">
      RE_Queue1
    </attribute>
    <attribute name="marking">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </node>
  <node id="4" nodeType="place">
    <attribute name="name">
      RE_Queue2
    </attribute>
    <attribute name="marking">
      <attribute name="expr"><attribute name="numValue">
        0
      </attribute></attribute>
    </attribute>
  </node>
  <node id="5" nodeType="place">
    <attribute name="name">
      AQueue2
    </attribute>
    <attribute name="marking">
      <attribute name="expr"><attribute name="name">
        R
      </attribute></attribute>
    </attribute>
  </node>
  <node id="6" nodeType="transition">
    <attribute name="weight">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="name">
      rho_0
    </attribute>
    <attribute name="service">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="expr"><attribute name="name">
          rho0
        </attribute></attribute>
      </attribute>
    </attribute>
  </node>
  <node id="7" nodeType="transition">
    <attribute name="weight">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="name">
      rho_1
    </attribute>
    <attribute name="service">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="expr"><attribute name="name">
          rho1
        </attribute></attribute>
      </attribute>
    </attribute>
  </node>
  <node id="8" nodeType="transition">
    <attribute name="weight">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="name">
      rho_2
    </attribute>
    <attribute name="service">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="expr"><attribute name="name">
          rho1
        </attribute></attribute>
      </attribute>
    </attribute>
  </node>
  <arc id="9" arcType="arc" source="6" target="3">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
  <arc id="10" arcType="arc" source="3" target="7">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
  <arc id="11" arcType="arc" source="7" target="4">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
  <arc id="12" arcType="arc" source="4" target="8">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
  <arc id="13" arcType="arc" source="8" target="5">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
  <arc id="14" arcType="arc" source="5" target="7">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
</model>