## Introduction

<div style="text-align: justify">
Private Information Retrieval, also known as covert search, is a very practical technology and application in secure multi-party computing that can be used to protect users' query privacy and thus their query results. The goal is to ensure that when users submit query requests to the data source, they complete the query without being perceived or leaked. For the data source, they only know that a query has arrived, but they do not know the true query conditions, so they do not know what the other party has checked.
<br>
<br>
<br>

![box](../../Image/Benchmark/Private Information Retrival/image1.png)
<style>
    img[alt="box"]{
        width:600px;
    }
</style>

<br>
<br>

**Query process**

1. The data stored by the data provider in the database is in the format of<key, value>;

2. The query party uses the key they want to query, encrypts it, and then goes to the data party to query the corresponding value;

3. During the query process, the data provider is unable to determine the specific key of the query provider, and it is not clear which value was ultimately sent to the query provider.

![im2](../../Image/Benchmark/Private Information Retrival/image2.jpg)
<style>
    img[alt="im2"]{
        width:800px;
    }
</style>

<br>
</div> 


## Application Flowchart
<br>
<br>

![flow](../../Image/Benchmark/Private Information Retrival/flow.png)
<style>
    img[alt="flow"]{
        width:1000px;
    }
</style>
<br>
<br>

## Matrix Construction Method
<div style="text-align: justify">
<br>
<br>

![m1](../../Image/Benchmark/Private Information Retrival/m1.jpg)
<style>
    img[alt="m1"]{
        width:1000px;
    }
</style>
<br>
<br>

![m2](../../Image/Benchmark/Private Information Retrival/m2.jpg)
<style>
    img[alt="m2"]{
        width:1000px;
    }
</style>
<br>
<br>

![m3](../../Image/Benchmark/Private Information Retrival/m3.jpg)
<style>
    img[alt="m3"]{
        width:1000px;
    }
</style>
<br>
</div> 
<br>
<br>

<!-- ## DEMO used by the system
<br>

**Integration with PrimiHub to complete PIR**

![demo](../../Image/Benchmark/Private Information Retrival/demo.jpg)
<style>
    img[alt="demo"]{
        width:1000px;
    }
</style>
<br>
<br>
-->

