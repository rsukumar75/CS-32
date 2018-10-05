void listAll(const Class* c, string path)  // two-parameter overload
{
    path = path + c->name();
    cout << path << endl ;
    if(c->subclasses().empty())
        return;
    vector<Class*> ::const_iterator it;
    it = c->subclasses().begin();
    path = path + "=>" ;
    for(;it!=c->subclasses().end();it++)
    {
        listAll(*it,path);
    }
}
