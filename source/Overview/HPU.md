# Homomorphic Computing Unit (HPU)

<div style="text-align: justify">


Poseidon is tightly coupled with the specialized hardware accelerator for FHE, namely **ZhangJiang No.1** or **ZJ-1** for simplicity. ZJ-1 is a PCIe interfaced SoC, providing highly parallelized computation for various FHE operations defined in Poseidon. It is a highly pipelined architecture with fine-grained operator executing on the timing-optimized microarchitectures. 

![ZJ1](../Image/Overview/ZJ1.png)

The performance of ZJ-1 is listed in the following table. It is evaluated using end-to-end ciphertext-computing benchmarks, including ID-based PIR, Framingham Heart Study (a cardiac disease prediction methodology) and bootstrap. It is worth mentioning that bootstrap usually works under high polynomial degree (i.e. 32768), so we do not evaluate this benchmark under degree 2048 ~ 8192.


**Performance——CKKS**

<tbody> 
<table>
  <tr style="width:1300px; text-align:center; vertical-align:middle;height:40px;">
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Application/Platform 
    </th>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;">
    CPU（s） 
    </th>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    ZJ-1（s）  
    </th>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Improvement     
    </th>
  </tr>
  <tr>
<th  colspan="4" style="width:1300px; text-align:center; vertical-align:middle;height:40px;">
Degree = 2048
</th>
 </tr>
  <tr style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Private Information Retrieval (PIR)
    </th >
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      53.748
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      1.486
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      36.1x
    </th>
  </tr>
 <tr>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Framingham Heart Study
    </th>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      1.987
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      0.093
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      21.4x
    </th>
  </tr>

  <tr>
<th  colspan="4" style="width:1300px; text-align:center; vertical-align:middle;height:40px;">
Degree = 4096
</th>
 </tr>
  <tr style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Private Information Retrieval (PIR)
    </th >
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      146.403
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      5.719
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      25.9x
    </th>
  </tr>
 <tr>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Framingham Heart Study
    </th>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      4.001
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      0.165
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      24.2x
    </th>
  </tr>

  <tr>
<th  colspan="4" style="width:1300px; text-align:center; vertical-align:middle;height:40px;">
Degree = 8192
</th>
 </tr>
  <tr style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Private Information Retrieval (PIR)
    </th >
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      512.426
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      26.773
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      20.7x
    </th>
  </tr>
 <tr>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Framingham Heart Study
    </th>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      8.237
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      0.355
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      23.2x
    </th>
  </tr>
  <tr>
<th  colspan="4" style="width:1300px; text-align:center; vertical-align:middle;height:40px;">
Degree = 16384
</th>
 </tr>
  <tr style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Private Information Retrieval (PIR)
    </th >
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      1751.006
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      93.864
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      18.6x
    </th>
  </tr>
 <tr>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Framingham Heart Study
    </th>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      16.749
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      0.689
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      24.3x
    </th>
  </tr>
 <tr>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Bootstrap
    </th>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      601.263
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      42.277
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      14.2x
    </th>
  </tr>
    <tr>
<th  colspan="4" style="width:1300px; text-align:center; vertical-align:middle;height:40px;">
Degree = 32768
</th>
 </tr>
  <tr style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Private Information Retrieval (PIR)
    </th >
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      5669.146
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      281.891
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      20.1x
    </th>
  </tr>
 <tr>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Framingham Heart Study
    </th>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      34.848
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      1.766883
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      19.7x
    </th>
  </tr>
 <tr>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Bootstrap
    </th>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      1367.626
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      83.134
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      16.4x
    </th>
  </tr>
 </tbody> 
</table>
<br>

**Performance——BFV**

<tbody> 
<table>
<tr style="width:1300px; text-align:center; vertical-align:middle;height:40px;">
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Application/Platform 
    </th>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;">
    CPU（s） 
    </th>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    ZJ-1（s）  
    </th>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Improvement     
    </th>
  </tr>
  <tr>
<th  colspan="4" style="width:1300px; text-align:center; vertical-align:middle;height:40px;">
Degree = 4096
</th>
 </tr>
  <tr style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Private Information Retrieval (PIR)
    </th >
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      0.1898
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      0.0081
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      16.1x
    </th>
  </tr>

<tr>
<th  colspan="4" style="width:1300px; text-align:center; vertical-align:middle;height:40px;">
Degree = 8192
</th>
 </tr>
  <tr style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Private Information Retrieval (PIR)
    </th >
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      0.80467
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      0.0136
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      23.4x
    </th>
  </tr>

<tr>
<th  colspan="4" style="width:1300px; text-align:center; vertical-align:middle;height:40px;">
Degree = 16384
</th>
 </tr>
  <tr style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Private Information Retrieval (PIR)
    </th >
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      3.3973
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      0.0346
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      59.1x
    </th>
  </tr>


  <tr>
<th  colspan="4" style="width:1300px; text-align:center; vertical-align:middle;height:40px;">
Degree = 32768
</th>
 </tr>
  <tr style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
    Private Information Retrieval (PIR)
    </th >
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      13.2914
    </td>
    <td style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      0.1080
    </td>
    <th style="width:1300px; text-align:center; vertical-align:middle;height:40px;" >
      98.2x
    </th>
  </tr>

</tbody>
</table>

<br>

Currently, ZJ-1 serves as the FPGA-based accelerator card. We believe that the performance is supposed to be boosted up to 1 magnitude if the subsequent ASIC is successfully taped out. 


</div>
