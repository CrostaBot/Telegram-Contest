# Telegram Contest
Language recognition and topic modelling based on the content of the Telegram channels.

## The Task
The task in this contest is to create a C/C++ library that can determine the language and topic of a Telegram channel.

**Determine channel language.** The algorithm must use the channel's name, description and the text of several posts to determine its language and return the language's 2-letter ISO code (or “other” if the language doesn’t have a two-letter code).

**Determine channel topic.** For channels in English and Russian, the algorithm must determine the relative weight for each of the topics identified in the channel. The topics are 41.

## Workflow

1. **Input filtering.** The dataset is given as a json containing three keys: Title, Description and Rececent Posts. Each key contains string values. In this first step I read the input and I clear the string from emojis, multiple space, link etc.

2. **Language Regognition.** In this step I performed the language recognition using a pretrained model of fasttext by Facebook (https://fasttext.cc/docs/en/language-identification.html). As output: dataset with languge label, dataset with all english string and finally a dataset with all ru string.

3. **Topic modelling.** After testing multiple unsupervised clustering algorithm, to implement this task I choose a semisupervised learning technique. This is because the dataset as a lots of spam and a lots of outliers. I labeled the 5% (about 1000 samples) of the eng dataset, trying to get an uniform distribution of the various topic. With the labeled dataset this speps are performed:
  1. Supervised training with the fasttext library (https://fasttext.cc/docs/en/supervised-tutorial.html), to create a text classifier model for our task.
  2.  This model is used to predict labels on the rest of the english dataset.
  3.  New training of the model with the whole dataset.

Russian topic modelling is done as explained below:
  1. Translation of the russian dataset in english
  2. Predict labels using the eng pretrained model 
  3. Train a new supervised model for russian topics.

## How to build
