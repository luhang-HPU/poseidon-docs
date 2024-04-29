## Introduction

<div style="text-align: justify">

Logistic Regression is a generalized linear model mainly used in binary classification. It works as a classifier utilizing Logistic function (also named Sigmoid function). Logistic Regression Classifier aims at learning a binary classification model from the features of training data. The training process is to construct a prediction function with Logistic function and to iterate the loss within each epoch.

![prediction](1.png)
<style>
    img[alt="prediction"]{
        width:250px;
    }
</style> 

<br>
<br>


## Derivation of the gradient

The logarithm of Loss function:
![loss](2.png)
<style>
    img[alt="loss"]{
        width:500px;
    }
</style>

Then, we have the gradient:
![gradient](3.png)
<style>
    img[alt="gradient"]{
        width:600px;
    }
</style>
<br>

## Vectorization Method
<br>

![vectorization](4.png)
<style>
    img[alt="vectorization"]{
        width:600px;
    }
</style>

We encode and encrypt each $x_i$ into a ciphertext, calculated with the encrypted weight $[\theta_i]$ to get the loss.
Then update the weight:

![update](5.png)
<style>
    img[alt="update"]{
        width:600px;
    }
</style>