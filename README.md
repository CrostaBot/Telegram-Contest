# Telegram Contest
Language recognition and topic modelling based on the content of the Telegram channels.

## The Task
The task in this contest is to create a C/C++ library that can determine the language and topic of a Telegram channel.

**Determine channel language.** The algorithm must use the channel's name, description and the text of several posts to determine its language and return the language's 2-letter ISO code (or “other” if the language doesn’t have a two-letter code).

**Determine channel topic.** For channels in English and Russian, the algorithm must determine the relative weight for each of the topics identified in the channel. The topics are 41:

+ Art & Design
+ Bets & Gambling
+ Books
+ Business & Entrepreneurship
+ Cars & Other Vehicles
+ Celebrities & Lifestyle
+ Cryptocurrencies
+ Culture & Events
+ Curious Facts
+ Directories of Channels & Bots
+ Economy & Finance
+ Education
+ Erotic Content
+ Fashion & Beauty
+ Fitness
+ Food & Cooking
+ Foreign Languages
+ Health & Medicine
+ History
+ Home & Architecture
+ Humor & Memes
+ Investments
+ Job Listings
+ Kids & Parenting
+ Marketing & PR
+ Motivation & Self-Development
+ Movies
+ Music
+ Offers & Promotions
+ Pets
+ Politics & Incidents
+ Psychology & Relationships
+ Real Estate
+ Recreation & Entertainment
+ Religion & Spirituality
+ Science
+ Sports
+ Technology & Internet
+ Travel & Tourism
+ Video Games
+ Other

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
1. Download the [source data](https://data-static.usercontent.dev/dc0202-input.tar.gz) and put it into "Dataset" folder
2. Run [dataset](https://github.com/CrostaBot/Telegram-Contest/blob/main/Dataset/Dataset.ipynb) notebook
3. Download the [fasttext pretrained model](https://dl.fbaipublicfiles.com/fasttext/supervised-models/lid.176.bin) and put it into "Language Recognition" folder
4. Run [Language Recognition](https://github.com/CrostaBot/Telegram-Contest/blob/main/Language%20Recognition/Language%20Recognition%20Dataset.ipynb) notebook
5. Download the [labeled eng dataset](https://docs.google.com/spreadsheets/d/1D8uMJSC6qkpPTNwG4hZ5asd5tQcI5kgKoO_uzcQBhes/edit?usp=sharing) and [further labels](https://docs.google.com/spreadsheets/d/14w6y28_oJE_UGTwyxUbzV7LGe29ZBeFvfX5C8cUAjFI/edit?usp=sharing) from russian dataset and the [russian dataset](https://docs.google.com/spreadsheets/d/1RGWC-y1ViM6MBP30pYW15Yvqxg2JHdbs8pxJHwq2Mz0/edit?usp=sharing). Then put them into "Topic Modelling" folder
6. Run in sequence [English](https://github.com/CrostaBot/Telegram-Contest/blob/main/Topic%20Modelling/ENG%20Topic%20Modelling.ipynb) and [Russian](https://github.com/CrostaBot/Telegram-Contest/blob/main/Topic%20Modelling/RU%20Topic%20Modelling.ipynb) topic modelling notebooks.

After these steps, all machine learning classifier are created. Create a folder "Models" with the trained models and put it libtgcat/resources.
If you do not want to create the models yourself, just [download them](https://drive.google.com/drive/folders/1QQaeurZfBZfU2FA2ZtLXYoK-a5M_E97L?usp=sharing).

To build the C++ library use the following command inside the libgcat folder:

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
make
```

To finally test the library download the [tester](https://data-static.usercontent.dev/libtgcat-tester.tar.gz) and run the following command:

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

Copy the resource folder and the libtgcat.so in the libtgcat, put the input data in the build folder. Then run:

```
make
tgcat-tester <mode> <input_file> <output_file>
```

where mode can be "language" for language identification or "category" for topic modelling.

## Contacts
+ Telegram: [@WuolterCox](https://t.me/WuolterCox)
