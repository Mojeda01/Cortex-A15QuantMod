# Regression: Predicting and Relating Quantitative Features

source: Advanced Data Analysis from an Elementary Point of View

## 1.1 Statistics, Data Analysis, Regression

-   Statistics is a branch of mathematical engineering that develops methods for making reliable inferences from imperfect data. Unlike sciences that study aspects of the world, statistics focuses on tools to help other sciences handle data, which is often incomplete, or noisy. Over the past two centuries, statistics has modeled these imperfections using probability, which is now central to its methods.

-   Statistics uses probability to understand and improve inference procedures - evaluating their reliability, error rates, and efficiency under various conditions. Key tools like regression help analyze relationships between variables. Regression also links to prediction, testing our understanding of these relationships and addressing "what if?" scenarios.

-   This book builds on foundational concepts of linear regression, exploring advanced methods for understanding and predicting relationships between variables, and positioning regression within broader family of smoothing techniques.

## 1.2 Guessing the Value of a Random Variable

-   To predict a quantitative random variable $Y$, we aim to guess a single value, referred to as the **optimal point forecast**. Determining this requires defining a function to evaluate the quality of our guesses. A common choice is the **mean squared error** (MSE), which measures the average squared difference predictions and the true values. This provides a starting point for assessing and optimizing prediction.

    $$
    MSE(m)\equiv\mathbb{E}\Big[(Y-m)^2\Big]\\
    =(\mathbb{E}[Y-m])^2+\mathbb{V}[Y-m]\\
    =(\mathbb{E}[Y-m])^2+\mathbb{V}[Y]\\
    =(\mathbb{E}[Y]-m)^2+\mathbb{V}[Y]
    $$

-   Notice that only the first, bias-squared term depends on our prediction \$m\$. We want to find the derivative of the MSE with respect to our prediction \$m\$, and then set that to zero at the optimal prediction $\mu$:

    $$
    \frac{dMSE}{dm}=-2(\mathbb{E}[Y]-m)+0\\
    \frac{dMSE}{dm}\Bigg|_{m=\mu}=0\\
    2(\mathbb{E}[Y]-\mu)=0\\
    \mu=\mathbb{E}[Y]
    $$

-   So, if we gauge the quality of our prediction by mean-squared error, the best prediction to make is the expected value.
