
class Book extends Obj {
	constructor(arg = false)
	{
		super("/api/book", arg);
	}
    
    icon(){
        return this.data.icon;
    }

    title(){
        return this.data.title;
    }
    
    brief(){
        return this.data.brief;
    }
};

function toBooks(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Book(x));
    return ret;
}
