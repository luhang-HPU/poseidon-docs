## Introduction

<div style="text-align: justify">
Framingham Heart Study (FHS) is the longest duration cardiovascular epidemiological study funded by the National Institutes of Health in 1948, aimed at improving understanding of the epidemiology of coronary heart disease in the United States. FHS indicates that cardiovascular disease is closely related to various factors such as age, blood pressure, cholesterol, height, weight, etc. Therefore, these health-related parameters can be used to predict the probability of cardiovascular disease.
<br>
<br>
<br>

![f1](f1.png)
<style>
    img[alt="f1"]{
        width:800px;
    }
</style>

<br>
<br>

**Prediction process**

1. Users input their own sign parameters and encrypt them

2. The processor performs encrypted calculations on the encrypted data<br>
**x** = 0.072∙Age+0.013∙SBP-0.029∙DBP+0.008∙CHL-0.053∙height+0.021∙weight<br>

3. Users decrypt the calculation results to obtain a prediction of their probability of developing cardiovascular diseases<br>**Prediction**：e^x/(1+e^x)<br>

</div>
<br>
<br>

## Application Flowchart
<br>
<br>

![ff](flowf.png)
<style>
    img[alt="ff"]{
        width:800px;
    }
</style>

<br>
<br>

