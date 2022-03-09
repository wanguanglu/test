#!/usr/bin/env python3

from pandas_datareader import wb
import numpy as np
import pandas as pd

pd.options.display.width = 0

# Download data from World Bank
names = [
    "NE.EXP.GNFS.CD",  # Exports of goods and services (current US$)
    "NE.IMP.GNFS.CD",  # Imports of goods and services (current US$)
    "NV.AGR.TOTL.CD",  # Agriculture, forestry, and fishing, value added (current US$)
    "NY.GDP.MKTP.CD",  # GDP (current US$)
    "NE.RSB.GNFS.CD",  # External balance on goods and services (current US$)
]
df = wb.download(country="all", indicator=names, start=2010,
                 end=2010).reset_index()

# We remove aggregates and keep only countries with no missing data
countries = wb.get_countries()
non_aggregates = countries[countries["region"] != "Aggregates"].name
df_nonagg = df[df["country"].isin(non_aggregates)].dropna()

# Extract vector for each country
vectors = {}
for rowid, row in df_nonagg.iterrows():
    vectors[row["country"]] = row[names].values

# Compute the Euclidean and cosine distances
euclid = {}
cosine = {}

target = "Australia"
for country in vectors:
    vecA = vectors[target]
    vecB = vectors[country]
    dist = np.linalg.norm(vecA - vecB)
    cos = (vecA @ vecB) / (np.linalg.norm(vecA) * np.linalg.norm(vecB))
    euclid[country] = dist  # Euclidean distance
    cosine[country] = 1 - cos  # cosine distance

# Print the results
df_distance = pd.DataFrame({"euclid": euclid, "cos": cosine})
print("Closest by Euclidean distance:")
print(df_distance.sort_values(by="euclid").head())
print()
print("Closest by Cosine distance:")
print(df_distance.sort_values(by="cos").head())

# Print the detail metrics
print()
print("Detail metrics:")
print(df_nonagg[df_nonagg.country.isin(["Mexico", "Colombia", "Australia"])])
