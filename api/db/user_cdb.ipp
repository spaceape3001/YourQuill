////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


namespace yq {
    namespace cdb {
        namespace {
            inline std::string user_filename(std::string_view k)
            {
                return make_filename(k, User::szExtension);
            }

            Vector<User>     all_users_sorted()
            {
                static thread_local SQ    s("SELECT id FROM Users ORDER BY k");
                return s.vec<User>();
            }
            
            Vector<User>     all_users_unsorted()
            {
                static thread_local SQ    s("SELECT id FROM Users");
                return s.vec<User>();
            }
        }

        Vector<User>       all_users(Sorted sorted)
        {
            return sorted ? all_users_sorted() : all_users_unsorted();
        }
        
        size_t                  all_users_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Users");
            return s.size();
        }

        bool                    any_users()
        {
            static thread_local SQ s("SELECT 1 FROM Users LIMIT 1");
            auto af = s.af();
            return s.step() == SqlQuery::Row;
        }

        std::string             brief(User u)
        {
            static thread_local SQ s("SELECT brief FROM Users WHERE id=?");
            return s.str(u.id);
        }

        User                    db_user(Document doc, bool* wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(folder(doc) != users_folder()){
                yError() << "Rejecting user " << key(doc) << " due to not being in the '.users' folder";        
                return User{};
            }
            std::string k   = base_key(doc);
            if(k.empty())
                return User();
            
            static thread_local SQ    i("INSERT OR FAIL INTO Users (k,id) VALUES (?,?)");
            auto i_lk   = i.af();
            
            i.bind(1, k);
            i.bind(2, doc.id);
            if(i.step(false) == SqlQuery::Done){
                if(wasCreated)
                    *wasCreated = true;
                return User{doc.id};
            } else if(exists_user(doc.id)){
                return User{doc.id};
            } else {
                yError() << "Unable to get the user from the database: " << k;
                return User{};
            }
        }
        
        User               db_user(Fragment f, bool* wasCreated)
        {
            return db_user(document(f),wasCreated);
        }

        User                db_user(std::string_view k, bool* wasCreated)
        {
            std::string     tfn = user_filename(k);
            return db_user(db_document(users_folder(), tfn), wasCreated);
        }
            

        Document                document(User u)
        {
            return exists(u) ? Document{u.id} : Document{};
        }
    
        void                    erase(User u)
        {
            static thread_local SQ d("DELETE FROM Users WHERE id=?");
            d.exec(u.id);
        }
        
        bool                    exists(User u)
        {
            return exists_user(u.id);
        }

        bool                    exists_user(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Users WHERE id=? LIMIT 1");
            return s.present(i);
        }
        
        Image                   icon(User u)
        {
            static thread_local SQ    s("SELECT icon FROM Users WHERE id=? LIMIT 1");
            return s.as<Image>(u.id);
        }
        
        User::Info              info(User u, bool autoKeyToName)
        {
            User::Info  ret;
            static thread_local SQ  s("SELECT k,name,icon,brief,is_admin,is_owner,is_reader,is_writer FROM Users WHERE id=?");
            auto s_af = s.af();
            s.bind(1, u.id);
            if(s.step() == SqlQuery::Row){
                ret.key     = s.v_text(1);
                ret.name    = s.v_text(2);
                ret.doc     = Document{u.id};
                ret.icon    = Image(s.v_uint64(3));
                ret.brief   = s.v_text(4);
                if(autoKeyToName && ret.name.empty())
                    ret.name    = ret.key;
                ret.is_admin    = s.v_bool(5);
                ret.is_owner    = s.v_bool(6);
                ret.is_reader   = s.v_bool(7);
                ret.is_writer   = s.v_bool(8);
            }
            return ret;
        }
        

        bool                    is_admin(User u)
        {
            static thread_local SQ s("SELECT is_admin FROM Users WHERE id=?");
            return s.boolean(u.id);
        }
        
        bool                    is_owner(User u)
        {
            static thread_local SQ s("SELECT is_owner FROM Users WHERE id=?");
            return s.boolean(u.id);
        }
        
        bool                    is_reader(User u)
        {
            static thread_local SQ s("SELECT is_reader FROM Users WHERE id=?");
            return s.boolean(u.id);
        }
        
        bool                    is_writer(User u)
        {
            static thread_local SQ s("SELECT is_writer FROM Users WHERE id=?");
            return s.boolean(u.id);
        }
        

        std::string             key(User u)
        {
            static thread_local SQ    s("SELECT k FROM Users WHERE id=?");
            return s.str(u.id);
        }
        

        User                    make_user(std::string_view k, const Root* rt)
        {
            if(!rt)
                rt  = wksp::root_first(DataRole::Users);
            if(!rt){
                yError() << "No root specified to create the user in!";
                return User{};
            }
            
            std::string     tfn = user_filename(k);
            Document    doc = db_document(users_folder(), tfn);
            bool            was = false;
            User         t   = db_user(doc, &was);
            if(!was)
                return t;
            if(fragments_count(doc))
                return t;
            User::SharedFile td  = write(t, rt);
            td -> name  = k;
            td -> save();
            return t;
        }
        

        User::SharedData         merged(User u, unsigned int opts)
        {
            User::SharedData  ret = std::make_shared<User::Data>();
            for(auto& i : reads(u)){
                if(opts & IsUpdate)
                    cdb::update(i.first);
                ret -> merge(*(i.second), static_cast<bool>(opts&Override));
            }
            return ret;
        }
        
        std::string             name(User u)
        {
            static thread_local SQ    s("SELECT name FROM Users WHERE id=?");
            return s.str(u.id);
        }

        Vector<UserFragDoc> reads(User u)
        {
            Vector<UserFragDoc>  ret;
            for(Fragment f : fragments(document(u), DataRole::Users)){
                User::SharedFile  p   = user_doc(f);
                if(p)
                    ret.push_back(UserFragDoc(f,p));
            }
            return ret;
        }
        
        Vector<UserFragDoc> reads(User u, class Root*rt)
        {
            Vector<UserFragDoc>  ret;
            for(Fragment f : fragments(document(u), rt)){
                User::SharedFile  p   = user_doc(f);
                if(p)
                    ret.push_back(UserFragDoc(f,p));
            }
            return ret;
        }

        User                    user(std::string_view k)
        {
            static thread_local SQ    s("SELECT id FROM Users WHERE k=?");
            return s.as<User>(k);
        }
        
        User                    user(uint64_t i)
        {
            return exists_user(i) ? User{i} : User{};
        }
        
        User::SharedFile        user_doc(Fragment f, bool fAllowEmpty)
        {
            if(!f)
                return User::SharedFile();

            auto ch   = frag_bytes(f);
            if(ch.empty())
                return fAllowEmpty ? std::make_shared<User::File>() : User::SharedFile();
            
            User::SharedFile  td = std::make_shared<User::File>();
            std::filesystem::path   fp  = path(f);
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to read " << fp;
                return User::SharedFile();
            }
            td -> set_file(fp);
            return td;
        }
        

        User::SharedFile          write(User u, const Root*rt)
        {
            Document    d   = document(u);
            if(!d)
                return User::SharedFile();
            if(rt && !rt->is_writable(DataRole::Users))
                return User::SharedFile();
            Fragment    f   = rt ? fragment(d, rt) : writable(d, DataRole::Users);
            if(f)
                return user_doc(f, true);

            Folder      fo  = folder(d);
            if((fo != cdb::users_folder()) && !exists(fo, rt))
                make_directory(fo, rt);

            User::SharedFile ptr  = std::make_shared<User::File>();
            ptr -> set_file( rt -> resolve(key(d)));
            ptr -> reload();
            return ptr;
        }    
    }
}
