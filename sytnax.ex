
#Luis Felipe Loera A01571914
#  Syntax highlighter

defmodule CppLexer do
  # This function takes a string of C++ code and returns a list of tokens. The tokens are categorized as comments, strings, keywords, identifiers, numbers, operators, and punctuation.
  @keywords ~w(
    auto break case catch char class const continue default do double else enum
    float for if int long namespace private protected public return short signed
    sizeof static struct switch template this throw try typedef typename union
    unsigned using virtual void while override include define string cout cin endl
    std main
    )
  def tokenize(code) do
    regex =
      # This regex matches comments, strings, keywords, identifiers, numbers, operators, and punctuation. In order to make it more readable,
      #we use the x flag to allow for whitespace and comments in the regex.
      ~r/(\/\/.*|\/\*[\s\S]*?\*\/|#.*|"([^"\\]|\\.)*"|'([^'\\]|\\.)'|
      \b(?:#{Enum.join(@keywords, "|")})\b|
      [a-zA-Z_][a-zA-Z0-9_]*|\d+(?:\.\d+)?|\+\+|--|==|!=|<=|>=|&&|\|\||<<|>>[+\-*\/%=<>]|
      [;,\(\)\{\}\[\]])
      /x
      # The regex is explained as follows:
      # the first part matches comments, which can be either single-line (//...) or multi-line (/*...*/).
      # the second part matches preprocessor directives, which start with # and continue until the end of the line.
      # the third part matches string literals, which are enclosed in double quotes and can contain escaped characters.
      # the fourth part matches character literals, which are enclosed in single quotes and can also contain escaped characters.
      # the fifth part matches keywords, which are defined in the @keywords list and are matched as whole words using \b.
      # the sixth part matches identifiers, which start with a letter or underscore and can be followed by letters, digits, or underscores.
      # the seventh part matches numbers, which can be integers or floating-point numbers.
      # the eighth part matches operators, which can be multi-character (like ++, --, ==, etc.) or single-character (like +, -, *, etc.).


    Regex.scan(regex, code) # This line uses the Regex.scan function to find all matches of the regex in the input code and returns a list of tokens.

  end
  def classify(token) do
    cond do
       #comments
       Regex.match?(~r/^(\/\/.*|\/\*[\s\S]*?\*\/)$/, token) ->
        :comment
      #preprocessor
      # The prepocessor helps us to include libraries, like iostream, and to define constants, like PI.
      Regex.match?(~r/^#.*$/, token) ->
        :preprocessor
      #keywords
      token in @keywords ->
        :keyword
      #strings
      Regex.match?(~r/^"([^"\\]|\\.)*"$/, token) ->
        :string
      #characters
      Regex.match?(~r/^'([^'\\]|\\.)'$/, token) ->
        :char
      #identifiers
      Regex.match?(~r/^[a-zA-Z_][a-zA-Z0-9_]*$/, token) ->
        :identifier
      #numbers
      Regex.match?(~r/^\d+(?:\.\d+)?$/, token) ->
        :number
      #operators
      Regex.match?(~r/^(\+\+|--|==|!=|<=|>=|&&|\|\||<<|>>|[+\-*\/%=<>])$/, token) ->
        :operator
      #delimeters
      Regex.match?(~r/^[;,\(\)\{\}\[\]]$/, token) ->
        :delimiter
        # If the token does not match any of the above categories, we classify it as :unknown.
      true ->
        :unknown

    end
  end
# This function takes a string of C++ code and generates an HTML document with syntax highlighting.
#It uses the tokenize and classify functions to identify the tokens in the code and wrap them in <span> elements with appropriate CSS classes for styling.
  def generate_html(code) do

    regex =
      ~r/(\/\/.*|\/\*[\s\S]*?\*\/|#.*|"([^"\\]|\\.)*"|'([^'\\]|\\.)'|
      \b(?:#{Enum.join(@keywords, "|")})\b|
      [a-zA-Z_][a-zA-Z0-9_]*|
      \d+(?:\.\d+)?|
      \+\+|--|==|!=|<=|>=|&&|\|\||<<|>>|
      [+\-*\/%=<>]|
      [;,\(\)\{\}\[\]]|
      \s+)/x

    tokens =
      Regex.scan(regex, code)
      |> Enum.map(fn [token | _] -> token end)

    body =
      Enum.map(tokens, fn token ->

        if String.match?(token, ~r/^\s+$/) do
          token
        else
          class = classify(token)
          "<span class=\"#{class}\">#{token}</span>"
        end

      end)
      |> Enum.join("")
    # Finally, we wrap the body of the HTML document in a basic structure with a link to a CSS file for styling.
    # The CSS file should define styles for the different token classes (comment, preprocessor, keyword, string, char, identifier, number, operator, delimiter) to achieve the desired syntax highlighting effect.

    """
    <html>
    <head>
      <link rel="stylesheet" href="token_colors.css">
    </head>

    <body>
      <pre>
  #{body}
      </pre>
    </body>
    </html>
    """
  end

  # And lastly, we have a function that reads a C++ source file, then generates the HTML with syntax highlighting, and saves it to an output file.
  def save_html(input_file, output_file) do
    {:ok, code} = File.read(input_file)
    html = generate_html(code)
    File.write(output_file, html)
  end
end
CppLexer.save_html("example.cpp", "salida.html")
