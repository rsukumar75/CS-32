
class Investment
{
public:
    Investment(string n, int p)
    :m_name(n),m_price(p)
    {}
    virtual ~Investment()
    {}
    virtual bool fungible() const
    {
        return false;
    }
    string name() const
    {
        return m_name;
    }
    virtual string description() const =0;
    int purchasePrice() const
    {
        return m_price;
    }
private:
    string m_name;
    int m_price;
    
};

class Painting : public Investment
{
public:
    Painting(string n, int p)
    : Investment(n,p)
    {}
    ~Painting()
    {
        cout << "Destroying " << name() << ", a " << description() << endl;
    }
    string description() const
    {
        return "painting";
    }
};

class Stock : public Investment
{
public:
    Stock(string n,int p, string t)
    : Investment(n,p),m_ticker(t)
    {}
    ~Stock()
    {
         cout << "Destroying " << name() << ", a stock holding" << endl;
    }
    bool fungible() const
    {
        return true;
    }
    string description() const
    {
        return ("stock trading as " + m_ticker);
    }
private:
    string m_ticker;
    
};

class House : public Investment
{
public:
    House(string n, int p)
    :Investment(n,p)
    {}
    ~House()
    {
         cout << "Destroying the " << description() << " " << name() << endl;
    }
    string description() const
    {
        return "house";
    }
};


