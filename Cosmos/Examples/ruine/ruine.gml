<?xml version="1.0" encoding="UTF-8"?>

<model formalismUrl="http://alligator.lip6.fr/sptgd-net.fml" xmlns="http://gml.lip6.fr/model">
  <attribute name="declarations">
    <attribute name="realConstDeclaration">
      <attribute name="constName">
        p
      </attribute>
      <attribute name="realFormula">
        <attribute name="value">
          0.3
        </attribute>
      </attribute>
    </attribute>
    <attribute name="realConstDeclaration">
      <attribute name="constName">
        q
      </attribute>
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </attribute>
  <node id="3" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="4" nodeType="transition">
    <attribute name="weight">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
    </attribute>
    <attribute name="service">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="realFormula">
          <attribute name="realConst">
            q
          </attribute>
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="5" nodeType="transition">
    <attribute name="weight">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
    </attribute>
    <attribute name="service">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="realFormula">
          <attribute name="realConst">
            q
          </attribute>
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="6" nodeType="transition">
    <attribute name="weight">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
    </attribute>
    <attribute name="service">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="realFormula">
          <attribute name="realConst">
            p
          </attribute>
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="7" nodeType="place">
    <attribute name="name">
      J1_1
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          0
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="8" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          0
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="9" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="10" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          0
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="11" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          0
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="12" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="13" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="14" nodeType="place">
    <attribute name="name">
      J2_5
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          0
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="15" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="17" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          0
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="16" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          0
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="19" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="18" nodeType="place">
    <attribute name="name">
      J1_5
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          0
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="21" nodeType="transition">
    <attribute name="weight">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
    </attribute>
    <attribute name="service">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="realFormula">
          <attribute name="realConst">
            p
          </attribute>
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="20" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="23" nodeType="transition">
    <attribute name="weight">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
    </attribute>
    <attribute name="service">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="realFormula">
          <attribute name="realConst">
            q
          </attribute>
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="22" nodeType="transition">
    <attribute name="weight">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
    </attribute>
    <attribute name="service">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="realFormula">
          <attribute name="realConst">
            q
          </attribute>
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="25" nodeType="transition">
    <attribute name="weight">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
    </attribute>
    <attribute name="service">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="realFormula">
          <attribute name="realConst">
            p
          </attribute>
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="24" nodeType="transition">
    <attribute name="weight">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
    </attribute>
    <attribute name="service">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="realFormula">
          <attribute name="realConst">
            q
          </attribute>
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="27" nodeType="transition">
    <attribute name="weight">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
    </attribute>
    <attribute name="service">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="realFormula">
          <attribute name="realConst">
            p
          </attribute>
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="26" nodeType="transition">
    <attribute name="weight">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
    </attribute>
    <attribute name="service">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="realFormula">
          <attribute name="realConst">
            p
          </attribute>
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="29" nodeType="transition">
    <attribute name="weight">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
    </attribute>
    <attribute name="service">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="realFormula">
          <attribute name="realConst">
            q
          </attribute>
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="28" nodeType="transition">
    <attribute name="weight">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="realFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
    </attribute>
    <attribute name="service">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        EXPONENTIAL
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="realFormula">
          <attribute name="realConst">
            p
          </attribute>
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="31" nodeType="place">
    <attribute name="name">
      J2_1
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          0
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="30" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="34" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="32" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </node>
  <node id="33" nodeType="place">
    <attribute name="name">
    </attribute>
    <attribute name="marking">
      <attribute name="intFormula">
        <attribute name="value">
          0
        </attribute>
      </attribute>
    </attribute>
  </node>
  <arc id="35" arcType="arc" source="31" target="4">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="38" arcType="arc" source="33" target="4">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="39" arcType="arc" source="4" target="32">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="36" arcType="arc" source="33" target="6">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="37" arcType="arc" source="34" target="4">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="42" arcType="arc" source="21" target="10">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="43" arcType="arc" source="21" target="8">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="40" arcType="arc" source="10" target="25">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="41" arcType="arc" source="17" target="29">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="46" arcType="arc" source="12" target="21">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="47" arcType="arc" source="6" target="30">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="44" arcType="arc" source="11" target="27">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="45" arcType="arc" source="6" target="32">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="51" arcType="arc" source="24" target="17">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="50" arcType="arc" source="30" target="21">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="49" arcType="arc" source="27" target="13">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="48" arcType="arc" source="8" target="6">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="55" arcType="arc" source="17" target="27">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="54" arcType="arc" source="5" target="8">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="53" arcType="arc" source="26" target="17">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="52" arcType="arc" source="5" target="33">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="59" arcType="arc" source="28" target="19">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="58" arcType="arc" source="8" target="23">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="57" arcType="arc" source="27" target="15">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="56" arcType="arc" source="20" target="28">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="63" arcType="arc" source="25" target="14">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="62" arcType="arc" source="29" target="19">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="61" arcType="arc" source="25" target="12">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="60" arcType="arc" source="19" target="26">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="68" arcType="arc" source="32" target="5">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="69" arcType="arc" source="16" target="28">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="70" arcType="arc" source="3" target="25">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="71" arcType="arc" source="15" target="26">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="64" arcType="arc" source="26" target="16">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="65" arcType="arc" source="24" target="11">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="66" arcType="arc" source="28" target="18">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="67" arcType="arc" source="29" target="15">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="76" arcType="arc" source="10" target="23">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="77" arcType="arc" source="16" target="29">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="78" arcType="arc" source="30" target="5">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="79" arcType="arc" source="13" target="24">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="72" arcType="arc" source="23" target="30">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="73" arcType="arc" source="15" target="24">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="74" arcType="arc" source="23" target="12">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="75" arcType="arc" source="11" target="22">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="85" arcType="arc" source="9" target="22">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="84" arcType="arc" source="22" target="13">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="87" arcType="arc" source="28" target="12">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="86" arcType="arc" source="27" target="32">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="81" arcType="arc" source="32" target="27">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="80" arcType="arc" source="26" target="8">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="83" arcType="arc" source="22" target="7">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="82" arcType="arc" source="8" target="26">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
  <arc id="88" arcType="arc" source="12" target="28">
    <attribute name="valuation">
      <attribute name="intFormula">
        <attribute name="value">
          1
        </attribute>
      </attribute>
    </attribute>
  </arc>
</model>