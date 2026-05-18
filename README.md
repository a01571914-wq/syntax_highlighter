# syntax_highlighter
1. Target langugage
The selected target language for this project is C++.
I select C++  because it contains a very wide variety of lexical elements such as keywords, identifiers, operators, literals, comments, and preprocessor directives, making it suitable for lexical analysis and perfect for tjis project

The development language used was Elixir, a functional programming language that allows concise pattern matching and efficient text processing.

2. Lexical categories
This lexical analyzer is used to identify this following lexical categories:
| Token Type              | Description                                   | Example     |
| ----------------------- | --------------------------------------------- | ----------- |
| Keywords                | Reserved words with predefined meaning in C++ | int, while  |
| Identifiers             | Names defined by the programmer               |  main       |
| Numbers                 | Integer or floating-point numeric literals    | 10, 3.14    |
| Strings                 | Text enclosed in double quotes                | "Hello"     |
| Characters              | Single characters enclosed in single quotes   | 'a'         |
| Operators               | Symbols used for operations                   | +, ==, &&   |
| Delimiters              | Symbols used for grouping or separation       | ;, { , }   |
| Comments                | Single-line or multi-line comments            | // comment  |
| Preprocessor            | Compiler instructions beginning with #        | #include    |

3. Lexical description:
The elixir program uses regular expresions (regex) to recognize  each token caregory:
Keywords:
Keywords are matched using a predefined list:
int,float,while,return,class,etc.

Identifiers:
Identifiers begin with a letter or underscore and may contain numbers afterward.
(a-zA-Z, a-zA-Z0-9)

Numbers:
Matches integers and floating-point values.
\d+(?:\.\d+)?

Strings:
Matches text enclosed in double quotes.
"(^"\\|\\.)"

Character Literals:
Matches characters enclosed in single quotes.
"(^'\\|\\.)"

Operators:
Matches arithmetic, logical, and comparison operators.
\+\+|--|==|!=|<=|>=|&&|\|\||<<|>>|+\-*\/%=<>

Delimiters:
Matches punctuation symbols used in C++.
;,\(\)\{\}\[\]

Comments:
Single-line comments
\/\/.*
Multi-line comments
\/\*\s\S*?\*\/

Preprocessor 
#.*

4. Program implementation
For this project we use elixir tio make a lexical analyzer   using:

- Regular expressions (Regex)
- Pattern matching
- File handling functions

Then the elixir program  the following steps:

- It reads a C++ source file.
- We use regular expressions to tokenize the code.
- Every token is classified according to its lexical category.
- As an output it is genrated an html document, that shows the tokens colored.
- Finally the CSS is created to color or highlight each token type.

The syntax highlighter outputs an html file where uses the css as:

- Keywords appear in red
- Comments appear in green
- Strings appear in pink
- Numbers appear in purple
- Operators appear in cyan

This improves readability and helps us to understand and simulate the behavior of modern code editors.

5. Algorithms Used

The main algorithm used for this elixir program is based on regular expression scanning, the analyzer traverses the source code and matches tokens sequentially using this function: Regex.scan(regex, code)
After the tokenization is done, each of the tokens applied are classified independently through conditional checks using:cond do
Finally, as it is asked the elixir program generates an html by iterating through the token list and wrapping each token in a <span> tag with a CSS class, where each token are highlighted as the css says.

6. Time complexity analysis

In this case for the program we declare that n = is the number of characters and t= is the number of tokens taht were identified, now that this have been stated in order to know the tokenization complexity is important to know that this lexical scanner processess the input sequentially so we have a complexity of O(n) because each character is visited approximately once during regex.
Now we add another constant:
k = is the number of lexical categories used for classification
For the token classification complexity, after the source code has already been tokenized, the program evaluates every token individually to determine its type. This is done inside the classify function using several regular expression comparisons and conditional checks.
Since each token may be compared against all lexical categories, the complexity can initially be expressed as:
O(t * k). However, the value of k is very small and constant because the lexer always uses the same predefined categories regardless of the file size. Because of this, the complexity simplifies to: O(t). This means the classification stage grows linearly with the number of tokens identified in the program.
Now for the html generation complexity, the program iterates through the token list one time in order to wrap every token inside a <span> tag with its corresponding CSS class. Since each token is processed only once, the complexity is: O(t).So it menas that the html generation stage is also linear with respect to the number of tokens in the source code.

7. Overall Complexity
Combining all the stages of the program we obtain the following complexity: O(n) + O(t) + O(t) where:
O(n) corresponds to the tokenization process,the first O(t) corresponds to the token classification stage, and the second O(t) corresponds to the HTML generation stage
Since the number of tokens identified is proportional to the size of the input file, we can say that: t ≈ n. this means that as the number of characters increases, the number of generated tokens also increases in the same proportion.
By replacing t with n, the total complexity becomes: O(n), Tthenthe program runs in linear time relative to the size of the input file. This indicates that the execution time grows proportionally with the amount of source code being analyzed, making the lexer efficient for both small and large C++ files.

8. Comparison with Practical Execution
The theoretical complexity analysis matches the practical behavior of the program, beacause when we test larger C++ files execution time increased proportionally with file size. This confirms that the lexer is efficient for normal source code analysis tasks.

9. Reflection on Ethical Implications
Lexical analyzers and syntax highlighting technologies are widely used nowadays in software development tools, compilers, among others. These technologies improve productivity, readability, and learning for programmers, but however software analysis tools can also raise some ethical concerns because having advanced code analysis systems may be used to ,analyze proprietary code without permission, also automation tools based on lexical analysis are increasingly integrated into artificial intelligence systems. Yes it is clear that these tools help developers write better software, but they may also reduce opportunities for beginner programmers to develop problem-solving skills.
For these reasons, it is important to use programming tools responsibly, respecting privacy, intellectual property, and academic integrity.

10. References
GeeksforGeeks. (2017, November 14). Time Complexity with Simple Examples. GeeksforGeeks. https://www.geeksforgeeks.org/dsa/understanding-time-complexity-simple-examples/

Dixon, D. (2019, September 2). Learning Regex with Elixir. Medium. https://medium.com/@SomeDude_Drew/learning-regex-with-elixir-d65488621076
‌
GeeksforGeeks. (2023, August 21). C++ Tokens. GeeksforGeeks. https://www.geeksforgeeks.org/cpp/cpp-tokens/
‌
Ethical issues - Impacts of digital technology on wider society - Eduqas - GCSE Computer Science Revision - Eduqas - BBC Bitesize. (2019). BBC Bitesize. https://www.bbc.co.uk/bitesize/guides/zd726yc/revision/1
‌
‌
