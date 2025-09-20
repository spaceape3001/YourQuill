
class Class extends Obj {
	constructor(arg = false)
	{
		super("/api/class", arg);
	}
    
    icon(){
        return this.data.icon;
    }

    title(){
        return this.data.name;
    }
    
    brief(){
        return this.data.brief;
    }
};

function toClasses(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Class(x));
    return ret;
}

