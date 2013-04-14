#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {
  struct cstring;
  struct string;
  struct lstring;
  template<typename T> inline const char* to_string(T);

  struct cstring {
    inline operator const char*() const;
    inline unsigned length() const;
    inline bool operator==(const char*) const;
    inline bool operator!=(const char*) const;
    inline optional<unsigned> position(const char *key) const;
    inline optional<unsigned> iposition(const char *key) const;
    inline cstring& operator=(const char *data);
    inline cstring(const char *data);
    inline cstring();

  protected:
    const char *data;
  };

  struct string {
    //deprecated: use string text = file::read(filename);
    inline static string read(const string &filename);

    inline static string date();
    inline static string time();
    inline static string datetime();

    inline void reset();
    inline void reserve(unsigned);
    inline void resize(unsigned);
    inline void clear(char);
    inline bool empty() const;

    template<typename... Args> inline string& assign(Args&&... args);
    template<typename... Args> inline string& append(Args&&... args);

    inline bool readfile(const string&);

    template<unsigned Limit = 0> inline string& replace(const char*, const char*);
    template<unsigned Limit = 0> inline string& ireplace(const char*, const char*);
    template<unsigned Limit = 0> inline string& qreplace(const char*, const char*);
    template<unsigned Limit = 0> inline string& iqreplace(const char*, const char*);

    inline unsigned length() const;
    inline unsigned capacity() const;

    template<unsigned Limit = 0> inline lstring split(const char*) const;
    template<unsigned Limit = 0> inline lstring isplit(const char*) const;
    template<unsigned Limit = 0> inline lstring qsplit(const char*) const;
    template<unsigned Limit = 0> inline lstring iqsplit(const char*) const;

    inline bool equals(const char*) const;
    inline bool iequals(const char*) const;

    inline bool wildcard(const char*) const;
    inline bool iwildcard(const char*) const;

    inline bool beginswith(const char*) const;
    inline bool ibeginswith(const char*) const;
    inline bool endswith(const char*) const;
    inline bool iendswith(const char*) const;

    inline string& lower();
    inline string& upper();
    inline string& qlower();
    inline string& qupper();
    inline string& transform(const char *before, const char *after);
    inline string& reverse();

    template<unsigned limit = 0> inline string& ltrim(const char *key = " ");
    template<unsigned limit = 0> inline string& rtrim(const char *key = " ");
    template<unsigned limit = 0> inline string& trim(const char *key = " ", const char *rkey = nullptr);
    inline string& strip();

    inline optional<unsigned> position(const char *key) const;
    inline optional<unsigned> iposition(const char *key) const;
    inline optional<unsigned> qposition(const char *key) const;
    inline optional<unsigned> iqposition(const char *key) const;

    inline explicit operator bool() const;
    inline operator const char*() const;
    inline char* operator()();
    inline char& operator[](int);

    inline bool operator==(const char*) const;
    inline bool operator!=(const char*) const;
    inline bool operator< (const char*) const;
    inline bool operator<=(const char*) const;
    inline bool operator> (const char*) const;
    inline bool operator>=(const char*) const;

    inline string& operator=(const string&);
    inline string& operator=(string&&);

    template<typename... Args> inline string(Args&&... args);
    inline string(const string&);
    inline string(string&&);
    inline ~string();

    inline char* begin() { return &data[0]; }
    inline char* end() { return &data[length()]; }
    inline const char* begin() const { return &data[0]; }
    inline const char* end() const { return &data[length()]; }

    //internal functions
    inline string& assign_(const char*);
    inline string& append_(const char*);

  protected:
    char *data;
    unsigned size;

    template<unsigned Limit, bool Insensitive, bool Quoted> inline string& ureplace(const char*, const char*);

  #if defined(QSTRING_H)
  public:
    inline operator QString() const;
  #endif
  };

  struct lstring : vector<string> {
    inline optional<unsigned> find(const char*) const;
    inline string concatenate(const char*) const;
    inline void append() {}
    inline void isort();
    template<typename... Args> inline void append(const string&, Args&&...);

    template<unsigned Limit = 0> inline lstring& split(const char*, const char*);
    template<unsigned Limit = 0> inline lstring& isplit(const char*, const char*);
    template<unsigned Limit = 0> inline lstring& qsplit(const char*, const char*);
    template<unsigned Limit = 0> inline lstring& iqsplit(const char*, const char*);

    inline bool operator==(const lstring&) const;
    inline bool operator!=(const lstring&) const;

    inline lstring& operator=(const lstring&);
    inline lstring& operator=(lstring&);
    inline lstring& operator=(lstring&&);

    template<typename... Args> inline lstring(Args&&... args);
    inline lstring(const lstring&);
    inline lstring(lstring&);
    inline lstring(lstring&&);

  protected:
    template<unsigned Limit, bool Insensitive, bool Quoted> inline lstring& usplit(const char*, const char*);
  };

  //compare.hpp
  inline char chrlower(char c);
  inline char chrupper(char c);
  inline int istrcmp(const char *str1, const char *str2);
  inline bool strbegin(const char *str, const char *key);
  inline bool istrbegin(const char *str, const char *key);
  inline bool strend(const char *str, const char *key);
  inline bool istrend(const char *str, const char *key);

  //convert.hpp
  inline char* strlower(char *str);
  inline char* strupper(char *str);
  inline char* qstrlower(char *str);
  inline char* qstrupper(char *str);
  inline char* strtr(char *dest, const char *before, const char *after);

  //format.hpp
  template<signed precision = 0, char padchar = ' '> inline string format(const string &value);
  template<signed precision = 0, char padchar = '0'> inline string hex(uintmax_t value);
  template<signed precision = 0, char padchar = '0'> inline string octal(uintmax_t value);
  template<signed precision = 0, char padchar = '0'> inline string binary(uintmax_t value);

  //math.hpp
  inline bool strint(const char *str, int &result);
  inline bool strmath(const char *str, int &result);

  //platform.hpp
  inline string activepath();
  inline string realpath(const string &name);
  inline string userpath();
  inline string configpath();
  inline string sharedpath();
  inline string temppath();

  //strm.hpp
  inline unsigned strmcpy(char *target, const char *source, unsigned length);
  inline unsigned strmcat(char *target, const char *source, unsigned length);
  inline bool strccpy(char *target, const char *source, unsigned length);
  inline bool strccat(char *target, const char *source, unsigned length);
  inline void strpcpy(char *&target, const char *source, unsigned &length);

  //strpos.hpp
  inline optional<unsigned> strpos(const char *str, const char *key);
  inline optional<unsigned> istrpos(const char *str, const char *key);
  inline optional<unsigned> qstrpos(const char *str, const char *key);
  inline optional<unsigned> iqstrpos(const char *str, const char *key);
  template<bool Insensitive = false, bool Quoted = false> inline optional<unsigned> ustrpos(const char *str, const char *key);

  //trim.hpp
  template<unsigned limit = 0> inline char* ltrim(char *str, const char *key = " ");
  template<unsigned limit = 0> inline char* rtrim(char *str, const char *key = " ");
  template<unsigned limit = 0> inline char* trim(char *str, const char *key = " ", const char *rkey = nullptr);
  inline char* strip(char *s);

  //utility.hpp
  template<bool Insensitive> alwaysinline bool chrequal(char x, char y);
  template<bool Quoted, typename T> alwaysinline bool quoteskip(T *&p);
  template<bool Quoted, typename T> alwaysinline bool quotecopy(char *&t, T *&p);
  inline string substr(const char *src, unsigned start = 0, unsigned length = ~0u);
  inline string sha256(const uint8_t *data, unsigned size);

  inline char* integer(char *result, intmax_t value);
  inline char* decimal(char *result, uintmax_t value);

  //these functions are deprecated, use format() instead:
  template<unsigned length = 0, char padding = ' '> inline string integer(intmax_t value);
  template<unsigned length = 0, char padding = ' '> inline string linteger(intmax_t value);
  template<unsigned length = 0, char padding = ' '> inline string decimal(uintmax_t value);
  template<unsigned length = 0, char padding = ' '> inline string ldecimal(uintmax_t value);
  inline unsigned fp(char *str, long double value);
  inline string fp(long double value);

  //variadic.hpp
  template<typename... Args> inline void print(Args&&... args);

  //wildcard.hpp
  inline bool wildcard(const char *str, const char *pattern);
  inline bool iwildcard(const char *str, const char *pattern);
};

#endif
