
class Tag extends Obj {
	constructor(arg = false)
	{
		super("/api/tag", arg);
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

    url(){
        return `/tag?id=${this.id}`;
    }
};


function toTags(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Tag(x));
    return ret;
}
