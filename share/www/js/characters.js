
class Character extends Obj {
	constructor(arg = false)
	{
		super("/api/character", arg);
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
        return `/character?id=${this.id}`;
    }
};

function toCharacters(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Character(x));
    return ret;
}
