<?xml version="1.0" encoding="UTF-8"?>

<model formalismUrl="http://alligator.lip6.fr/sptgd-net.fml" xmlns="http://gml.lip6.fr/model">
  <attribute name="declaration">
    <attribute name="constants">
      <attribute name="intConsts">
      </attribute>
      <attribute name="realConsts">
        <attribute name="realConst">
          <attribute name="name">
            rho1
          </attribute>
          <attribute name="expr">
            <attribute name="numValue">
              0.45
            </attribute>
          </attribute>
        </attribute>
        <attribute name="realConst">
          <attribute name="name">
            rho0
          </attribute>
          <attribute name="expr">
            <attribute name="numValue">
              0.1
            </attribute>
          </attribute>
        </attribute>
      </attribute>
    </attribute>
  </attribute>
  <node id="3" nodeType="place">
    <attribute name="name">
      Queue1
    </attribute>
    <attribute name="marking">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </node>
  <node id="4" nodeType="place">
    <attribute name="name">
      Queue2
    </attribute>
    <attribute name="marking">
      <attribute name="expr"><attribute name="numValue">
        0
      </attribute></attribute>
    </attribute>
  </node>
  <node id="5" nodeType="transition">
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
      rho0
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
      rho1
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
      rho2
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
  <arc id="8" arcType="arc" source="5" target="3">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
  <arc id="9" arcType="arc" source="3" target="6">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
  <arc id="10" arcType="arc" source="6" target="4">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
  <arc id="11" arcType="arc" source="4" target="7">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
</model>